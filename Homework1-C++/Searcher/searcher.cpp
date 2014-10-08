#include "searcher.h"

using namespace searcher;

Searcher::Searcher(QMultiHash<QString, QString> hashTable)
	: mHashTable(hashTable)
{
}

void Searcher::processRequest(QString const &request)
{
	QString const normalizedRequest = request.toLower();

	if (incorrectRequest(normalizedRequest)) {
		qDebug() << "Incorrect request";
		return;
	}

	if (normalizedRequest.contains("or")) {
		processOrRequest(normalizedRequest);
	} else {
		if (normalizedRequest.contains("and")) {
			processAndRequest(normalizedRequest);
		} else {
			processSimpleRequest(normalizedRequest);
		}
	}
}

bool Searcher::incorrectRequest(QString const &request)
{
	bool containsAnd = request.contains("and");
	bool containsOr = request.contains("or");
	bool containsTwoTypesOfOperators = containsAnd && containsOr;
	bool containsTooManyWords = (!containsAnd) && request.contains(" ") && (!containsOr);

	return containsTwoTypesOfOperators || containsTooManyWords;
}

void Searcher::processSimpleRequest(QString const &request)
{
	QStringList foundFiles = mHashTable.values(request);
	output(foundFiles);
}

void Searcher::processOrRequest(QString const &request)
{
	QString copy = request;
	QString requestWithoutSpaces = copy.replace(" ", "");
	QStringList parts = requestWithoutSpaces.split("or");

	QStringList result;

	for (QString const part : parts) {
		QStringList foundFiles = mHashTable.values(part);

		for (QString const &file : foundFiles) {
			if (!result.contains(file)) {
				result.append(file);
			}
		}
	}

	output(result);
}

void Searcher::processAndRequest(QString const &request)
{
	QString copy = request;
	QString requestWithoutSpaces = copy.replace(" ", "");
	QStringList parts = requestWithoutSpaces.split("and");

	QSet<QString> result = mHashTable.values().toSet();

	for (QString const part : parts) {
		QStringList foundFiles = mHashTable.values(part);
		result = setAndList(result, foundFiles);
	}

	output(result.toList());
}

void Searcher::output(QStringList list)
{
	if (list.isEmpty()) {
		qDebug() << "Nothing found";
	} else {
		for (QString const &word : list) {
			qDebug() << word << " ";
		}
	}
}

QSet<QString> Searcher::setAndList(QSet<QString> startSet, QStringList list)
{
	QSet<QString> result;

	for (QString const &word : startSet) {
		if (list.contains(word)) {
			result.insert(word);
		}
	}

	return result;
}
