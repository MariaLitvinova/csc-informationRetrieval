#include <QCoreApplication>
#include <QtCore/QDebug>

#include "stemmer.h"
#include "indexer.h"

using namespace indexer;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	if (argc != 3) {
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

	return a.exec();
}
