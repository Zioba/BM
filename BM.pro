#-------------------------------------------------
#
# Project created by QtCreator 2017-01-07T18:45:00
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BM
TEMPLATE = app


SOURCES += main.cpp\
        deldialog.cpp \
        mainwindow.cpp \
    logger.cpp \
    ipdialog.cpp \
    converter.cpp

HEADERS  += mainwindow.h\
        deldialog.h \
    logger.h \
    ipdialog.h \
    converter.h

FORMS    += mainwindow.ui\
        deldialog.ui \
    ipdialog.ui
