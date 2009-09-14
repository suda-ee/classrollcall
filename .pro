# -------------------------------------------------
# Project created by QtCreator 2009-03-22T20:09:25
# -------------------------------------------------
QT += opengl xml
TARGET = classrollcall
TEMPLATE = app
CONFIG += windows

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
win32:INCLUDEPATH += "d:/Program Files/Tegesoft/gics/include" "d:/Program Files/Tegesoft/camp/include" "d:/boost_1_40_0"
win32:LIBS += "d:\Program Files\Tegesoft\gics\lib\libgics.dll.a" "d:\Program Files\Tegesoft\camp\lib\libcamp.dll.a" -lAdvapi32
