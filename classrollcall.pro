# -------------------------------------------------
# Project created by QtCreator 2009-03-22T20:09:25
# -------------------------------------------------
QT += opengl xml
TARGET = classrollcall
TEMPLATE = app
CONFIG += windows mmx sse sse2
#CONFIG += console

CODECFORTR = GBK
QTPLUGIN += qcncodecs

# CONFIG += embed_manifest_exe
SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    inputpanel.cpp \
    outputpanel.cpp
HEADERS += view.hpp \
    inputpanel.hpp \
    outputpanel.hpp \
    mainwindow.hpp
# FORMS += mainwindow.ui \
RESOURCES += resources/resources.qrc
# TRANSLATIONS += rzpctrl_zh_CN.ts
win32:RC_FILE = classrollcall.rc
win32:INCLUDEPATH += "G:\Program Files (x86)\Tegesoft/gics/include" "G:\Program Files (x86)\Tegesoft/camp/include" "g:/boost_1_42_0"
win32:LIBS += "G:\Program Files (x86)\Tegesoft\gics\lib\gics.lib" "G:\Program Files (x86)\Tegesoft\camp\lib\camp.lib" -lAdvapi32
