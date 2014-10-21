#include "searcher.h"

using namespace searcher;

Searcher::Searcher(QMultiHash<QString, QString> hashTable)
	: mHashTable(hashTable)
{
}

QStringList Searcher::processRequest(QString const &request)
{
	QString const loweredRequest = request.toLower();

	if (incorrectRequest(loweredRequest) && (!loweredRequest.contains("/"))) {
		qDebug() << "Incorrect request";
		return {};
	}

	QString const normalizedRequest = requestWithoutCoordinateParts(loweredRequest);

	if (normalizedRequest.contains("or")) {
		return processOrRequest(normalizedRequest);
	} else {
		if (normalizedRequest.contains("and")) {
			return processAndRequest(normalizedRequest);
		} else {
			return processSimpleRequest(normalizedRequest);
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

QString Searcher::requestWithoutCoordinateParts(QString const &request)
{
	QStringList splittedList = request.split(" ");
	QString resultRequest = "";

	for (QString const &part : splittedList) {
		if (part.at(0) != '/') {
			resultRequest += part + " and ";
		}
	}

	return resultRequest;
}

QStringList Searcher::processSimpleRequest(QString const &request)
{
	QStringList foundFiles = mHashTable.values(request);
	output(foundFiles);
	return foundFiles;
}

QStringList Searcher::processOrRequest(QString const &request)
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
	return result;
}

QStringList Searcher::processAndRequest(QString const &request)
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
	return result.toList();
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
