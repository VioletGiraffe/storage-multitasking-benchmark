#include "assert/advanced_assert.h"
#include "benchmarkedfile/cbenchmarkedfile.h"

DISABLE_COMPILER_WARNINGS
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
RESTORE_COMPILER_WARNINGS

int main(int argc, char *argv[])
{
	AdvancedAssert::setLoggingFunc([](const char* message) {
		qDebug() << message;
	});

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

	CBenchmarkedFile file(1024 * 1024 * 100, CBenchmarkedFile::Read, [](int progress) {}); // 100 MB

    return app.exec();
}
