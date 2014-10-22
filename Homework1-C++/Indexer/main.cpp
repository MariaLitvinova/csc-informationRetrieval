#include <QCoreApplication>
#include <QtCore/QDebug>

#include "stemmer.h"
#include "indexer.h"
#include "coordinateIndexer.h"

using namespace indexer;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	if ((argc != 3) && (argc != 4)) {
		qDebug() << "Invalid arguments";
		return 1;
	}

	QString const pathToDocuments = argv[1];
	QString const pathToMyStem = argv[2];
	QString const applicationPath = a.applicationDirPath();

	Stemmer stemmer(pathToDocuments, pathToMyStem, applicationPath);
	stemmer.stem();

	Indexer indexer(applicationPath);
	indexer.buildIndex();

	if (argc == 4) {
		CoordinateIndexer coordIndexer(applicationPath);
		coordIndexer.buildCoordinateIndex();
	}
}
