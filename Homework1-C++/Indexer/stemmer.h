#pragma once

#include <QtCore/QString>
#include <QtCore/QProcess>
#include <QtCore/QDir>
#include <QtCore/QDebug>

namespace indexer {

class Stemmer
{
public:
	Stemmer(
			QString const &pathToDocuments
			, QString const &pathToMyStem
			, QString const &pathToApplication
			);
	void stem();

private:
	void stemOneFile(QFileInfo fileInfo);

	void createDirectoryForStemmedFiles();

	QString mPathToDocuments;
	QString mPathToStemmedDocuments;

	QString mPathToMyStem;
	QString mPathToApplication;
};
}
