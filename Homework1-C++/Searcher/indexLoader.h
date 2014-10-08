#pragma once

#include <QtCore/QString>
#include <QtCore/QMultiHash>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QTextCodec>

namespace searcher {

class IndexLoader
{
public:
	IndexLoader(QString const &pathToIndex);
	QMultiHash<QString, QString> loadedIndex();

private:
	QMultiHash<QString, QString> mHashTable;
};
}
