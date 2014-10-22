#include "indexLoader.h"

using namespace searcher;

IndexLoader::IndexLoader(QString const &pathToIndex)
{
	QFile file(pathToIndex);

	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "Failed opening index file";
		return;
	}

	qDebug() << "Starting loading index";
	QTextStream in(&file);
	in.setCodec(QTextCodec::codecForName("UTF-8"));

	while (!in.atEnd()) {
		QString const line = in.readLine();
		QStringList lstLine = line.split("@");
		mHashTable.insert(lstLine.first(), lstLine.last());
	}

	file.close();
	qDebug() << "Index loaded";
}

QMultiHash<QString, QString> IndexLoader::loadedIndex()
{
	return mHashTable;
}
