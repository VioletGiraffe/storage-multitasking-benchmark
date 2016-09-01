QT = core quick quickcontrols2 quickwidgets

TARGET = StorageMultitaskingBenchmark
TEMPLATE = app

CONFIG += c++14

mac* | linux*{
    CONFIG(release, debug|release):CONFIG += Release
    CONFIG(debug, debug|release):CONFIG += Debug
}

Release:OUTPUT_DIR=release
Debug:OUTPUT_DIR=debug

DESTDIR  = ../bin/$${OUTPUT_DIR}
OBJECTS_DIR = ../build/$${OUTPUT_DIR}/app
MOC_DIR     = ../build/$${OUTPUT_DIR}/app
UI_DIR      = ../build/$${OUTPUT_DIR}/app
RCC_DIR     = ../build/$${OUTPUT_DIR}/app

LIBS += -L../bin/$${OUTPUT_DIR} -lcore -lcpputils

# Required for qDebug() to log function name, file and line in release build
DEFINES += QT_MESSAGELOGCONTEXT

win*{
    QMAKE_CXXFLAGS += /MP /wd4251
    QMAKE_CXXFLAGS_WARN_ON = /W4
    DEFINES += WIN32_LEAN_AND_MEAN NOMINMAX _SCL_SECURE_NO_WARNINGS
}

linux*|mac*{

    QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-c++11-extensions -Wno-local-type-template-args -Wno-deprecated-register

    CONFIG(release, debug|release):CONFIG += Release
    CONFIG(debug, debug|release):CONFIG += Debug

    Release:DEFINES += NDEBUG=1
    Debug:DEFINES += _DEBUG

    PRE_TARGETDEPS += $${DESTDIR}/libcore.a $${DESTDIR}/libcpputils.a
}

win32*:!*msvc2012:*msvc* {
    QMAKE_CXXFLAGS += /FS
}

INCLUDEPATH += \
    src \
    ../core \
    ../cpputils \
    ../cpp-template-utils

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SOURCES += \
    src/main.cpp

RESOURCES += \
    src/qml.qrc
