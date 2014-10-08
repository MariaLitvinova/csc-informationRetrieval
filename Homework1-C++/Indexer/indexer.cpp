#include "indexer.h"
#include "utils.h"

using namespace indexer;

Indexer::Indexer(QString const &pathToApplication)
	: mPathToIndex(pathToApplication + "\\index")
	, mPathToStemmed(pathToApplication + "\\stemmed")
{
}

void Indexer::buildIndex()
{
	createDirectoryForIndex();

	QDir dir(mPathToStemmed);
	if (!dir.exists()) {
		qDebug() << "Directory with stemmed files not found!" << mPathToStemmed;
		return;
	}

	qDebug() << "Starting building index..." << mPathToStemmed;

	for (QFileInfo const &fileInfo : dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		if (!fileInfo.isDir()) {
			buildIndexForOneFile(fileInfo);
		}
	}

	qDebug() << "Index built!";

	qDebug() << "Saving to file";
	saveIndexToFile();
}

void Indexer::buildIndexForOneFile(QFileInfo fileInfo)
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

	QString content = in.readAll().replace("{", "}").replace("|", "}");
	QStringList splittedContent = content.split("}");

	for (QString const &word : splittedContent) {
		addWordToIndex(word, fileName);
	}

	file.close();
}

void Indexer::addWordToIndex(QString const &word, QString const &fileName)
{
	if (!mHashTable.contains(word, fileName)) {
		mHashTable.insert(word, fileName);
	}
}

void Indexer::saveIndexToFile()
{
	QFile file(mPathToIndex + "/index.txt");

	if (!file.open(QIODevice::WriteOnly)) {
		qDebug() << "Failed opening index file";
		return;
	}

	QTextStream out(&file);
	out.setCodec(QTextCodec::codecForName("UTF-8"));

	QMultiHash<QString, QString>::iterator i;
	for (i = mHashTable.begin(); i != mHashTable.end(); ++i) {
		out << i.key();
		out << "@";
		out << i.value();
		out << "\r\n";
	}

	file.close();
	qDebug() << "Saved to file";
}

void Indexer::createDirectoryForIndex()
{
	Utils::createDir(mPathToIndex);
}
