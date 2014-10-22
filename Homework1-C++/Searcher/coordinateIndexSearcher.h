#pragma once

#include <QtCore/QString>
#include <QtCore/QMultiHash>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QTextCodec>

namespace searcher {

class CoordinateIndexSearcher
{
public:
	CoordinateIndexSearcher(QHash<QString, QMultiHash<QString, QString>> coordinateIndex);
	void processRequest(QStringList foundFiles, QString const &fullRequest);

private:
	bool okayForRequest(
			QString const &fileName
			, QString const &firstWord
			, QString const &secondWord
			, int const number
			, char compareOperator
			);

	char operatorOfComparing(QString const &request) const;
	int distanceBetweenWords(QString request) const;

	bool equalFound(int const firstWordIndex, int const secondWordIndex, int const number);
	bool furtherFound(int const firstWordIndex, int const secondWordIndex, int const number);
	bool closerFound(int const firstWordIndex, int const secondWordIndex, int const number);

	bool anyFound(QList<int> firstWordIndexes, QList<int> secondWordIndexes, char comparingOperator, int const number);

	QList<int> wordNumbers(QString const &fileName, QString const &word);

	QHash<QString, QMultiHash<QString, QString>> mHashTable;
};
}
