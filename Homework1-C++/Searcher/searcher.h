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
	void processRequest(QString const &request);

private:
	bool incorrectRequest(QString const &request);

	void processSimpleRequest(QString const &request);

	void processOrRequest(QString const &request);
	void processAndRequest(QString const &request);

	void output(QStringList list);

	QSet<QString> setAndList(QSet<QString> startSet, QStringList list);

	QMultiHash<QString, QString> mHashTable;
};
}
