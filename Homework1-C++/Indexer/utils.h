#pragma once

#include <QtCore/QString>
#include <QtCore/QDir>

namespace indexer {

class Utils
{
public:
	static void createDir(QString const &path);
	static void removeDir(QString const &path);
};
}
