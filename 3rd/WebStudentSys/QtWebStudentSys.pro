QT += core widgets webenginewidgets webchannel

CONFIG += c++17

TARGET = QtWebStudentSystem
TEMPLATE = app

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "Qt学习项目"
QMAKE_TARGET_PRODUCT = "Qt Web学生管理系统"

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    webbridge.cpp

HEADERS += \
    mainwindow.h \
    webbridge.h

RESOURCES += \
    resources.qrc

DESTDIR = $$PWD/bin
OBJECTS_DIR = $$PWD/build/obj
MOC_DIR = $$PWD/build/moc
RCC_DIR = $$PWD/build/rcc