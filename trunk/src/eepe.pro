HEADERS += mainwindow.h \
    file.h \
    pers.h \
    myeeprom.h \
    modeledit.h \
    generaledit.h \
    mdichild.h
SOURCES += main.cpp \
    mainwindow.cpp \
    file.cpp \
    pers.cpp \
    modeledit.cpp \
    generaledit.cpp \
    mdichild.cpp
RESOURCES += eepe.qrc

TARGET = eepe
TEMPLATE = app
FORMS += modeledit.ui \
    generaledit.ui


win32:RC_FILE += icon.rc
