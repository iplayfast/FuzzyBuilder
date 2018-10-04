#-------------------------------------------------
#
# Project created by QtCreator 2014-03-17T22:23:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = FuzzyBuilder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addlogic.cpp \
    TFuzzy.c \
    node.cpp \
    graphwidget.cpp \
    edge.cpp \
    qcustomplot.cpp \
    andnode.cpp \
    fuzzynode.cpp \
    innode.cpp \
    ornode.cpp \
    outnode.cpp \
    pidnode.cpp \
    nodefactory.cpp \
    viewsourcedialog.cpp \
    Util.cpp \
    boundnode.cpp \
    mapnode.cpp

HEADERS  += mainwindow.h \
    addlogic.h \
    node.h \
    TFuzzy.h \
    graphwidget.h \
    edge.h \
    qcustomplot.h \
    andnode.h \
    fuzzynode.h \
    innode.h \
    ornode.h \
    outnode.h \
    pidnode.h \
    nodefactory.h \
    viewsourcedialog.h \
    Util.h \
    boundnode.h \
    mapnode.h

FORMS    += mainwindow.ui \
    addlogic.ui \
    viewsourcedialog.ui
