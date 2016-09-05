#-------------------------------------------------
#
# Project created by QtCreator 2016-07-28T23:21:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml

TARGET = go
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    zoomleveldescription.cpp \
    imagedescription.cpp \
    graphicsitemgroup.cpp

HEADERS  += mainwindow.h \
    zoomleveldescription.h \
    imagedescription.h \
    graphicsitemgroup.h

FORMS    += mainwindow.ui
