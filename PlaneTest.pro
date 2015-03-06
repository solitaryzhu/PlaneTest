#-------------------------------------------------
#
# Project created by QtCreator 2015-02-14T20:59:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlaneTest
TEMPLATE = app

INCLUDEPATH += F:/Win_32bit/gdal/Release/include
LIBS += F:/Win_32bit/gdal/Release/lib/gdal_i.lib


SOURCES += main.cpp\
        mainwindow.cpp \
    myrectf.cpp \
    drawobject.cpp \
    paintarea.cpp

HEADERS  += mainwindow.h \
    myrectf.h \
    drawobject.h \
    paintarea.h

FORMS    += mainwindow.ui
