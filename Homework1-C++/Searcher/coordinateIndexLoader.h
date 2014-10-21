#pragma once

#include <QtCore/QString>
#include <QtCore/QMultiHash>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QTextCodec>
#include <QtCore/QHash>

namespace searcher {

class CoordinateIndexLoader
{
public:
	explicit CoordinateIndexLoader(QString const &coordinateIndexDirPath);
	QHash<QString, QMultiHash<QString, QString>> loadedIndex();

private:
	void loadOneFileIndex(QFileInfo fileInfo);

	QHash<QString, QMultiHash<QString, QString>> mHashTable;
};
}
