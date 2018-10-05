#-------------------------------------------------
#
# Project created by QtCreator 2014-03-17T22:23:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = FuzzyBuilder
TEMPLATE = app
INCLUDEPATH += inc/

SOURCES += src/addlogic.cpp \
    src/andnode.cpp \
    src/boundnode.cpp \
    src/edge.cpp \
    src/fuzzynode.cpp \
    src/graphwidget.cpp \
    src/innode.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mapnode.cpp \
    src/node.cpp \
    src/nodefactory.cpp \
    src/ornode.cpp \
    src/outnode.cpp \
    src/pidnode.cpp \
    src/qcustomplot.cpp \
    src/timernode.cpp \
    src/Util.cpp \
    src/viewsourcedialog.cpp \
    src/TFuzzy.c

HEADERS  += inc/mainwindow.h \
    inc/addlogic.h \
    inc/node.h \
    inc/TFuzzy.h \
    inc/graphwidget.h \
    inc/edge.h \
    inc/qcustomplot.h \
    inc/andnode.h \
    inc/fuzzynode.h \
    inc/innode.h \
    inc/ornode.h \
    inc/outnode.h \
    inc/pidnode.h \
    inc/nodefactory.h \
    inc/viewsourcedialog.h \
    inc/Util.h \
    inc/boundnode.h \
    inc/mapnode.h \
    inc/graphwidget.h

FORMS    += ui/mainwindow.ui \
    ui/addlogic.ui \
    ui/viewsourcedialog.ui
