#include "coordinateIndexLoader.h"

using namespace searcher;

CoordinateIndexLoader::CoordinateIndexLoader(QString const &coordinateIndexDirPath)
{
	QDir dir(coordinateIndexDirPath);
	if (!dir.exists()) {
		qDebug() << "Directory with coordinate files not found!" << coordinateIndexDirPath;
		return;
	}

	qDebug() << "Starting loading index..." << coordinateIndexDirPath;

	for (QFileInfo const &fileInfo : dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		if (!fileInfo.isDir()) {
			loadOneFileIndex(fileInfo);
			qDebug() << "Index for " << fileInfo.fileName() << " loaded!";
		}
	}
}

QHash<QString, QMultiHash<QString, QString> > CoordinateIndexLoader::loadedIndex()
{
	return mHashTable;
}

void CoordinateIndexLoader::loadOneFileIndex(QFileInfo fileInfo)
{
	QString const absoluteFilePath = fileInfo.absoluteFilePath();
	QString const fileName = fileInfo.fileName();

	QFile file(absoluteFilePath);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "Failed opening index file";
		return;
	}

	QTextStream in(&file);
	in.setCodec(QTextCodec::codecForName("IBM 866"));

	QMultiHash<QString, QString> newInsertion;

	while (!in.atEnd()) {
		QString const line = in.readLine();
		QStringList lstLine = line.split("@");
		newInsertion.insert(lstLine.first(), lstLine.last());
	}

	mHashTable.insert(fileName, newInsertion);

	file.close();
}
