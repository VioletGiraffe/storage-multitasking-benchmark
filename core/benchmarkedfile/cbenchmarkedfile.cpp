#include "cbenchmarkedfile.h"
#include "system/ctimeelapsed.h"
#include "assert/advanced_assert.h"

DISABLE_COMPILER_WARNINGS
#include <QDateTime>
#include <QDebug>
#include <QDir>
RESTORE_COMPILER_WARNINGS

#include <limits>

inline void generateRandomData(QByteArray& byteArray, const std::function<void(size_t)>& progressListener, std::minstd_rand& random_generator, std::uniform_int_distribution<uint32_t>& distribution)
{
	uint32_t* dataPtr = (uint32_t*)byteArray.data();
	for (size_t i = 0, count = byteArray.size() / 4, previousProgress = 0; i < count; ++i)
	{
		dataPtr[i] = distribution(random_generator);
		const size_t progress = i * 100 / count;
		if (progress != previousProgress)
		{
			previousProgress = progress;
			progressListener(progress);
		}
	}
}

CBenchmarkedFile::CBenchmarkedFile(size_t size, Direction direction, const std::function<void (int)>& initProgressListener) :
	//_file(QDir::tempPath() + '/' + QDateTime::currentDateTime().toString(Qt::ISODate).replace(':', '-') + ".tmp"),
	_file("D:/TEMP/" + QDateTime::currentDateTime().toString(Qt::ISODate).replace(':', '-') + ".tmp"),
	_size(size),
	_direction(direction),
	_initProgressListener(initProgressListener),
	_random_generator((unsigned int) time(nullptr))
{
	assert_and_return_message_r(
		_file.open(QFile::WriteOnly),
		std::string("Failed to open\n") + _file.fileName().toStdString() + "\nfor writing, error: " + _file.errorString().toStdString(),
		);

	_data.resize((int)_size);
	assert_r(_size % 4 == 0);

	if (direction == Read)
	{
		CTimeElapsed localTimer(true);

		QByteArray fileContentsData;
		fileContentsData.resize((int)_size);

		generateRandomData(fileContentsData, [this](size_t progress) {
			_initProgressListener((int)(progress * 3 / 4)); // Data generation / writing to file take roughly 75% and 25% total time
		}, _random_generator, _distribution);

		qDebug() << "Generating" << _size << "bytes of random data took" << localTimer.elapsed() << "ms";
		localTimer.start();

		assert_and_return_message_r((size_t)_file.write(fileContentsData) == _size, std::string("Failed to fill the file\n") + _file.fileName().toStdString(), );
		_file.close();

		_initProgressListener(100);

		qDebug() << "Writing" << _size << "bytes of random data took" << localTimer.elapsed() << "ms";

		assert_and_return_message_r(
			_file.open(QFile::ReadOnly),
			std::string("Failed to open\n") + _file.fileName().toStdString() + "\nfor reading",
			);
	}
	else if (direction == Write)
	{
		generateRandomData(_data, [this](size_t progress) {
			_initProgressListener((int)progress);
		}, _random_generator, _distribution);
	}
	else
		assert_unconditional_r("Having a bad day?");
}

CBenchmarkedFile::~CBenchmarkedFile()
{
	_file.close();
	assert_r(_file.remove());
}

void CBenchmarkedFile::setInitProgressListener(const std::function<void(int)>& listener)
{
	_initProgressListener = listener;
}

void CBenchmarkedFile::setOperationProgressListener(const std::function<void(int)>& listener)
{
	_operationProgressListener = listener;
}

uint64_t CBenchmarkedFile::runBenchmark()
{
	CTimeElapsed timer(true);

	timer.pause();
	const uint64_t testDuration = timer.elapsed<std::chrono::microseconds>();

	if (_direction == Read)
	{
		assert_and_return_message_r(_file.read(_data.data(), (qint64)_size) == _size, std::string("Failed to read the file") + _file.fileName().toStdString(), 0);
	}

	return (uint64_t)_size * 1000000ULL / testDuration;
}
