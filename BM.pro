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
    ipdialog.cpp \
    converter.cpp \
    dbworker.cpp \
    commandsaddform.cpp \
    utility.cpp \
    commandsmessagebox.cpp

HEADERS  += mainwindow.h\
        deldialog.h \
    ipdialog.h \
    converter.h \
    dbworker.h \
    commandsaddform.h \
    utility.h \
    commandsmessagebox.h

FORMS    += mainwindow.ui\
        deldialog.ui \
    ipdialog.ui \
    commandsaddform.ui

RESOURCES += \
    resource.qrc
