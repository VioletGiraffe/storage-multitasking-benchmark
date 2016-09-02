#pragma once

#include <QFile>

class CBenchmarkedFile
{
public:
	explicit CBenchmarkedFile(size_t size);
	CBenchmarkedFile& operator=(const CBenchmarkedFile&) = delete;

	bool init();

private:
	QFile _file;
	const size_t _size;
};
