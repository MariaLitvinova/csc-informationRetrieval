#include <QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QMultiHash>

#include <iostream>

#include "indexLoader.h"
#include "searcher.h"

using namespace searcher;
using namespace std;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	if (argc != 2) {
		qDebug() << "Invalid arguments";
		return 1;
	}

	setlocale(LC_CTYPE, "Russian");

	QString const pathToIndex = argv[1];

	IndexLoader loader(pathToIndex);
	QMultiHash<QString, QString> hashTable = loader.loadedIndex();

	if (hashTable.isEmpty()) {
		qDebug() << "Index is not loaded";
		return 1;
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

	return a.exec();
}
