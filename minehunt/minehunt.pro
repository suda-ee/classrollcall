TEMPLATE = app
TARGET  = minehunt
QT += declarative xml
CONFIG += qt plugin
# CONFIG += console

# Input
HEADERS += minehunt.h
SOURCES += main.cpp minehunt.cpp

sources.files = minehunt.qml minehunt.pro MinehuntCore

INSTALLS = target

RESOURCES += \
    minehunt.qrc
win32:INCLUDEPATH += "I:/boost_1_42_0"
win32:LIBS += -lAdvapi32
win32:RC_FILE = minehunt.rc
