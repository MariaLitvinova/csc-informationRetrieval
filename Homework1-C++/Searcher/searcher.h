#pragma once

#include <QtCore/QString>
#include <QtCore/QMultiHash>
#include <QtCore/QDebug>
#include <QtCore/QSet>

namespace searcher {

class Searcher
{
public:
	explicit Searcher(QMultiHash<QString, QString> hashTable);
	QStringList processRequest(QString const &request);

private:
	bool incorrectRequest(QString const &request);
	QString requestWithoutCoordinateParts(QString const &request);

	QStringList processSimpleRequest(QString const &request);

	QStringList processOrRequest(QString const &request);
	QStringList processAndRequest(QString const &request);

	void output(QStringList list);

	QSet<QString> setAndList(QSet<QString> startSet, QStringList list);

	QMultiHash<QString, QString> mHashTable;
};
}
