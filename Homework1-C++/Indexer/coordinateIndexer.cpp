#include "coordinateIndexer.h"
#include "utils.h"

using namespace indexer;

CoordinateIndexer::CoordinateIndexer(QString const &pathToApplication)
	: mPathToIndex(pathToApplication + "\\coordinateIndex")
	, mPathToStemmed(pathToApplication + "\\stemmed")
{
}

void CoordinateIndexer::buildCoordinateIndex()
{
	createDirectoryForCoordinateIndex();

	QDir dir(mPathToStemmed);
	if (!dir.exists()) {
		qDebug() << "Directory with stemmed files not found!" << mPathToStemmed;
		return;
	}

	qDebug() << "Starting building index..." << mPathToStemmed;

	for (QFileInfo const &fileInfo : dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		if (!fileInfo.isDir()) {
			buildIndexForOneFile(fileInfo);

			qDebug() << "Index for " << fileInfo.fileName() << " built!";

			qDebug() << "Saving to file...";
			saveIndexToFile(fileInfo.fileName());
		}
	}
}

void CoordinateIndexer::buildIndexForOneFile(QFileInfo fileInfo)
{
	QString const fullName = fileInfo.absoluteFilePath();
	QString const fileName = fileInfo.fileName();

	QFile file(fullName);

	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "Opening " << fileName << " failed";
		return;
	}

	QTextStream in(&file);
	in.setCodec(QTextCodec::codecForName("UTF-8"));

	QString content = in.readAll().replace("{", "}");
	QVector<QString> splittedContent = content.split("}").toVector();

	for (int i = 0; i < splittedContent.length(); ++i) {
		QString word = splittedContent[i];
		QStringList simpleWords = word.split("|");

		for (QString const &simpleWord : simpleWords) {
			addOneWordToIndex(simpleWord, i);
		}
	}

	file.close();
}

void CoordinateIndexer::addOneWordToIndex(QString const &word, const int number)
{
	if (!word.isEmpty()) {
		mWordAndNumber.insert(word, number);
	}
}

void CoordinateIndexer::createDirectoryForCoordinateIndex()
{
	Utils::createDir(mPathToIndex);
}

void CoordinateIndexer::saveIndexToFile(QString const &fileName)
{
	QFile file(mPathToIndex + "/" + fileName + "_index.txt");

	if (!file.open(QIODevice::WriteOnly)) {
		qDebug() << "Failed opening index file";
		return;
	}

	QTextStream out(&file);
	out.setCodec(QTextCodec::codecForName("UTF-8"));

	QMultiHash<QString, int>::iterator i;
	for (i = mWordAndNumber.begin(); i != mWordAndNumber.end(); ++i) {
		out << i.key();
		out << "@";
		out << i.value();
		out << "\r\n";
	}

	file.close();
	qDebug() << "Saved to file";
}
