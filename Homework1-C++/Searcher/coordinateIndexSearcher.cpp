#include "coordinateIndexSearcher.h"

using namespace searcher;

CoordinateIndexSearcher::CoordinateIndexSearcher(QHash<QString, QMultiHash<QString, QString>> coordinateIndex)
	: mHashTable(coordinateIndex)
{
}

void CoordinateIndexSearcher::processRequest(QStringList foundFiles, QString const &fullRequest)
{
	QStringList parsedList = fullRequest.split(" ");

	bool somethingFound = false;
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
			somethingFound = true;
		}
	}

	if (!somethingFound) {
		qDebug() << "Nothing found!";
	}
}

bool CoordinateIndexSearcher::okayForRequest(
		QString const &fileName
		, QString const &firstWord
		, QString const &secondWord
		, int const number
		, char compareOperator)
{
	QList<int> firstWordEntries = wordNumbers(fileName, firstWord);
	QList<int> secondWordEntries = wordNumbers(fileName, secondWord);

	return anyFound(firstWordEntries, secondWordEntries, compareOperator, number);
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

bool CoordinateIndexSearcher::equalFound(const int firstWordIndex, const int secondWordIndex, int const number)
{
	return abs(firstWordIndex - secondWordIndex) == number;
}

bool CoordinateIndexSearcher::furtherFound(const int firstWordIndex, const int secondWordIndex, const int number)
{
	return firstWordIndex - secondWordIndex >= number;
}

bool CoordinateIndexSearcher::closerFound(const int firstWordIndex, const int secondWordIndex, const int number)
{
	return firstWordIndex - secondWordIndex <= number;
}

bool CoordinateIndexSearcher::anyFound(QList<int> firstWordIndexes, QList<int> secondWordIndexes, char comparingOperator, const int number)
{
	bool anyFound = false;

	for (int const firstWordIndex : firstWordIndexes) {
		for (int const secondWordIndex : secondWordIndexes) {
			bool currentWordsFound = false;
			if (comparingOperator == '+') {
				currentWordsFound = furtherFound(firstWordIndex, secondWordIndex, number);
			} else {
				if (comparingOperator == '-') {
					currentWordsFound = closerFound(firstWordIndex, secondWordIndex, number);
				} else {
					currentWordsFound = equalFound(firstWordIndex, secondWordIndex, number);
				}
			}

			anyFound = anyFound || currentWordsFound;
		}
	}

	return anyFound;
}

QList<int> CoordinateIndexSearcher::wordNumbers(QString const &fileName, QString const &word)
{
	QStringList numbersInStr = mHashTable.value(fileName).values(word);

	QList<int> numbers;
	for (QString const &number : numbersInStr) {
		numbers.append(number.toInt());
	}

	return numbers;
}
