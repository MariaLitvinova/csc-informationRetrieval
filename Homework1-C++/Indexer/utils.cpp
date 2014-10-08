#include "utils.h"

using namespace indexer;

void Utils::createDir(QString const &path)
{
	QDir dir;

	if (dir.exists(path)) {
		dir.rmdir(path);
	}

	dir.mkdir(path);
}

void Utils::removeDir(QString const &path)
{

}
