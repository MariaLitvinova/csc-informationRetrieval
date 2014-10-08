#pragma once

#include <QtCore/QString>
#include <QtCore/QMultiHash>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QTextCodec>

namespace indexer {

class Indexer
{
public:
	explicit Indexer(QString const &pathToApplication);
	void buildIndex();

private:
	void buildIndexForOneFile(QFileInfo fileInfo);
	void addWordToIndex(QString const &word, QString const &fileName);

	void saveIndexToFile();

	void createDirectoryForIndex();

	QString mPathToStemmed;
	QString mPathToIndex;

	QMultiHash<QString, QString> mHashTable;
};
}
