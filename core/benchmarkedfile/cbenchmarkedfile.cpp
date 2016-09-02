#include "cbenchmarkedfile.h"

#include <QDateTime>
#include <QDir>

CBenchmarkedFile::CBenchmarkedFile(size_t size) :
	_file(QDir::tempPath() + '/' + QDateTime::currentDateTime().toString()),
	_size(size)
{

}

bool CBenchmarkedFile::init()
{
	return true;
}
