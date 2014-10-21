#include <QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QMultiHash>

#include <iostream>

#include "indexLoader.h"
#include "coordinateIndexLoader.h"
#include "searcher.h"
#include "coordinateIndexSearcher.h"

using namespace searcher;
using namespace std;

bool isCoordinateRequest(QString const &request)
{
	return request.contains("/");
}

void searchInSimpleIndex(QString const &pathToIndex)
{
	IndexLoader loader(pathToIndex);
	QMultiHash<QString, QString> hashTable = loader.loadedIndex();

	if (hashTable.isEmpty()) {
		qDebug() << "Index is not loaded";
	}

	qDebug() << "Requests: \n";
	QString line = "";

	Searcher search(hashTable);

	QTextStream in(stdin);
	in.setCodec(QTextCodec::codecForName("IBM 866"));

	do {
		line = in.readLine();
		if (line != ":q") {
			search.processRequest(line);
		}
	} while (line != ":q");
}

void searchInCoordinateIndex(QString const &pathToIndex, QString const &pathToCoordinateIndex)
{
	IndexLoader loader(pathToIndex);
	QMultiHash<QString, QString> hashTable = loader.loadedIndex();

	if (hashTable.isEmpty()) {
		qDebug() << "Index is not loaded";
	}

	CoordinateIndexLoader coordinateLoader(pathToCoordinateIndex);
	QHash<QString, QMultiHash<QString, QString>> coordinateHashTable = coordinateLoader.loadedIndex();

	if (coordinateHashTable.isEmpty()) {
		qDebug() << "Coordinate index is not loaded";
	}

	qDebug() << "Requests: \n";
	QString line = "";

	Searcher search(hashTable);
	CoordinateIndexSearcher coordinateSearch(coordinateHashTable);

	QTextStream in(stdin);
	in.setCodec(QTextCodec::codecForName("IBM 866"));

	do {
		line = in.readLine();
		if (line != ":q") {
			QStringList result = search.processRequest(line);
			coordinateSearch.processRequest(result, line);
		}
	} while (line != ":q");
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	if ((argc != 2) && (argc != 3)) {
		qDebug() << "Invalid arguments";
		return 1;
	}

	setlocale(LC_CTYPE, "Russian");

	if (argc == 2) {
		QString const pathToIndex = argv[1];
		searchInSimpleIndex(pathToIndex);
	} else {
		QString const pathToIndex = argv[1];
		QString const pathToCoordinateIndex = argv[2];
		searchInCoordinateIndex(pathToIndex, pathToCoordinateIndex);
	}

	return a.exec();
}
