INCLUDEPATH+=$$PWD
INCLUDEPATH+=$$PWD/inc
DEPENDPATH += $$PWD
 
SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/addlogic.cpp \
    src/TFuzzy.c \
    src/node.cpp \
    src/graphwidget.cpp \
    src/groups.cpp \
    src/edge.cpp \
    src/qcustomplot.cpp \
    src/andnode.cpp \
    src/fuzzynode.cpp \
    src/setupnode.cpp \
    src/innode.cpp \
    src/ornode.cpp \
    src/outnode.cpp \
    src/pidnode.cpp \
    src/nodefactory.cpp \
    src/viewsourcedialog.cpp \
    src/Util.cpp \
    src/boundnode.cpp \
    src/notnode.cpp \
    src/groupitem.cpp

HEADERS  += inc/mainwindow.h \
    inc/addlogic.h \
    inc/node.h \
    inc/TFuzzy.h \
    inc/graphwidget.h \
    inc/groups.h \
    inc/edge.h \
    inc/setupnode.h \
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
    inc/notnode.h \
    inc/groupitem.h

FORMS += ui/addlogic.ui \
	ui/groups.ui \
        ui/mainwindow.ui \
        ui/viewsourcedialog.ui
