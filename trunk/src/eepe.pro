HEADERS += mainwindow.h \
    file.h \
    pers.h \
    myeeprom.h \
    modeledit.h \
    generaledit.h \
    mdichild.h \
    helpers.h \
    edge.h \
    node.h \
    mixerdialog.h \
    burnconfigdialog.h
SOURCES += main.cpp \
    mainwindow.cpp \
    file.cpp \
    pers.cpp \
    modeledit.cpp \
    generaledit.cpp \
    mdichild.cpp \
    helpers.cpp \
    node.cpp \
    edge.cpp \
    mixerdialog.cpp \
    burnconfigdialog.cpp
RESOURCES += eepe.qrc
TARGET = eepe
TEMPLATE = app
FORMS += modeledit.ui \
    generaledit.ui \
    mixerdialog.ui \
    burnconfigdialog.ui
win32:RC_FILE += icon.rc
VERSION = $$system(svnversion)
VERSTR = '\\"$${VERSION}\\"' # place quotes around the version string
DEFINES += VER=\"$${VERSTR}\" # create a VER macro containing the version string
