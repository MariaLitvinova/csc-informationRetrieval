#include "stemmer.h"
#include "utils.h"

using namespace indexer;

Stemmer::Stemmer(
		QString const &pathToDocuments
		, QString const &pathToMyStem
		, QString const &pathToApplication)
	: mPathToDocuments(pathToDocuments)
	, mPathToMyStem(pathToMyStem)
	, mPathToApplication(pathToApplication)
	, mPathToStemmedDocuments(pathToApplication + "/stemmed")
{
}

void Stemmer::stem()
{
	createDirectoryForStemmedFiles();

	QDir dir(mPathToDocuments);
	if (!dir.exists()) {
		qDebug() << "Directory not found!" << mPathToDocuments;
		return;
	}

	qDebug() << "Starting stemming..." << mPathToDocuments;

	for (QFileInfo const &fileInfo : dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		if (!fileInfo.isDir()) {
			stemOneFile(fileInfo);
		}
	}

	qDebug() << "Stemming finished!";
}

void Stemmer::stemOneFile(QFileInfo fileInfo)
{
	QString const fileName = fileInfo.fileName();

	qDebug() << "Stemming: " << fileName;

	QString const firstArgument = fileInfo.absoluteFilePath();
	QString const secondArgument = mPathToStemmedDocuments + "/" + fileName;

	QStringList arguments;
	arguments << firstArgument << secondArgument;

	QProcess stemmer;
	stemmer.start(mPathToMyStem, arguments);
	stemmer.waitForFinished();
}

void Stemmer::createDirectoryForStemmedFiles()
{
	Utils::createDir(mPathToStemmedDocuments);
}
