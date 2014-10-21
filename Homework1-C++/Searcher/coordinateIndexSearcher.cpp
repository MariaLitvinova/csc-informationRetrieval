#include "coordinateIndexSearcher.h"

using namespace searcher;

CoordinateIndexSearcher::CoordinateIndexSearcher(QHash<QString, QMultiHash<QString, QString>> coordinateIndex)
	: mHashTable(coordinateIndex)
{
}

void CoordinateIndexSearcher::processRequest(QStringList foundFiles, QString const &fullRequest)
{
	QStringList parsedList = fullRequest.split(" ");

	for (QString const &file : foundFiles) {
		bool result = true;
		int counterInList = 0;

		for (QString const &part : parsedList) {
			if (part.at(0) == '/') {
				int distance = distanceBetweenWords(part);
				char compOperator = operatorOfComparing(part);

				result = result && okayForRequest(file, parsedList.at(counterInList - 1)
						, parsedList.at(counterInList + 1), distance, compOperator);
			}
			counterInList++;
		}
		if (result) {
			qDebug() << file;
		}
	}
}

bool CoordinateIndexSearcher::okayForRequest(
		QString const &fileName
		, QString const &firstWord
		, QString const &secondWord
		, int const number
		, char compareOperator)
{
	if (compareOperator == '\0') {
		return (wordNumber(fileName, firstWord) == wordNumber(fileName, secondWord) - number)
				||
				(wordNumber(fileName, secondWord) == wordNumber(fileName, firstWord) - number);
	} else {
		if (compareOperator == '-') {
			return (wordNumber(fileName, firstWord) - wordNumber(fileName, secondWord) <= number);
		} else {
			return (wordNumber(fileName, firstWord) - wordNumber(fileName, secondWord) >= number);
		}
	}
}

char CoordinateIndexSearcher::operatorOfComparing(QString const &request) const
{
	if (request.contains("+")) {
		return '+';
	} else {
		if (request.contains("-")) {
			return '-';
		} else {
			return '\0';
		}
	}
}

int CoordinateIndexSearcher::distanceBetweenWords(QString request) const
{
	return request.remove("/").remove("-").remove("+").toInt();
}

int CoordinateIndexSearcher::wordNumber(QString const &fileName, QString const &word)
{
	return mHashTable.value(fileName).value(word).toInt();
}
