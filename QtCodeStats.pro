#-------------------------------------------------
#
# Project created by QtCreator 2014-01-09T16:54:33
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtCodeStats
TEMPLATE = app

DESTDIR = $$PWD/dist

RC_FILE += $$PWD/img/ico.rc

SOURCES += \
    src/main.cpp\
    src/cmainwindow.cpp \
    src/cstatthread.cpp \
    src/caboutdialog.cpp

HEADERS  += \
    src/cmainwindow.h \
    src/cstatthread.h \
    src/caboutdialog.h

FORMS    += \
    src/cmainwindow.ui \
    src/caboutdialog.ui

OTHER_FILES += \
    LICENSE.GPL \
    README

RESOURCES += \
    img/img.qrc \
    translations/translations.qrc

TRANSLATIONS += \
    translations/qtcodestats_fr.ts
