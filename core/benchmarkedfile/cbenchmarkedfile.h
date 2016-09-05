#pragma once
#include "compiler/compiler_warnings_control.h"

DISABLE_COMPILER_WARNINGS
#include <QFile>
RESTORE_COMPILER_WARNINGS

#include <functional>
#include <random>
#include <vector>

class CBenchmarkedFile
{
public:
	enum Direction {Read, Write};

	CBenchmarkedFile(size_t size, Direction direction, const std::function<void(int)>& initProgressListener);
	~CBenchmarkedFile();

	CBenchmarkedFile& operator=(const CBenchmarkedFile&) = delete;

	void setInitProgressListener(const std::function<void(int)>& listener);
	void setOperationProgressListener(const std::function<void(int)>& listener);

	// Returns the speed in Bytes/second
	uint64_t runBenchmark();

private:
	QFile _file;
	const size_t _size;
	const Direction _direction;

	std::function<void (int)> _initProgressListener;
	std::function<void (int)> _operationProgressListener;

	std::minstd_rand _random_generator;
	std::uniform_int_distribution<uint32_t> _distribution;

	// Holds the data to be written by the write benchmark, and serves as the destination for the read benchmark
	QByteArray _data;
};
