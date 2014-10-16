#pragma once

#include <QtCore/QString>
#include <QtCore/QMultiHash>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QTextCodec>

namespace indexer {

class CoordinateIndexer
{
public:
	CoordinateIndexer(QString const &pathToApplication);
	void buildCoordinateIndex();

private:
	void buildIndexForOneFile(QFileInfo file);
	void addOneWordToIndex(QString const &word, int const number);

	void createDirectoryForCoordinateIndex();

	void saveIndexToFile(QString const &fileName);

	QString mPathToStemmed;
	QString mPathToIndex;

	QMultiHash<QString, int> mWordAndNumber;
};
}
