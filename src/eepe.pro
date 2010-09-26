HEADERS += mainwindow.h \
    file.h \
    pers.h \
    myeeprom.h \
    modeledit.h \
    generaledit.h
SOURCES += main.cpp \
    mainwindow.cpp \
    file.cpp \
    pers.cpp \
    modeledit.cpp \
    generaledit.cpp
RESOURCES += eepe.qrc

# install
# target.path = eepe
# sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS eepe.pro images
# sources.path = eepe
# INSTALLS += target sources
TARGET = eepe
TEMPLATE = app
FORMS += modeledit.ui \
    generaledit.ui
