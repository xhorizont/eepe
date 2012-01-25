QT += network \
      xml
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
    burnconfigdialog.h \
    avroutputdialog.h \ 
    simulatordialog.h \
    donatorsdialog.h \
    printdialog.h \
    preferencesdialog.h \
    fusesdialog.h \
    mixerslist.h \
    downloaddialog.h \
    stamp-eepe.h \
    customizesplashdialog.h
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
    burnconfigdialog.cpp \
    avroutputdialog.cpp \ 
    simulatordialog.cpp \
    donatorsdialog.cpp \
    printdialog.cpp \
    preferencesdialog.cpp \
    fusesdialog.cpp \
    mixerslist.cpp \
    downloaddialog.cpp \
    customizesplashdialog.cpp
RESOURCES += eepe.qrc
TARGET = eepe

unix {
LANGS.path = /usr/bin/eepe/lang
LANGS.files = lang/*.qm

ICON.path = /usr/bin/eepe
ICON.files += icon.svg

SHORTCUT.path = /usr/share/applications/
SHORTCUT.files += eepe.desktop

BINFILE.files += eepe
BINFILE.path = /usr/bin/eepe

INSTALLS = BINFILE
INSTALLS += LANGS
INSTALLS += ICON
INSTALLS += SHORTCUT
}

TEMPLATE = app
FORMS += modeledit.ui \
    generaledit.ui \
    mixerdialog.ui \
    burnconfigdialog.ui \
    avroutputdialog.ui \
    simulatordialog.ui \
    donatorsdialog.ui \
    printdialog.ui \
    preferencesdialog.ui \
    fusesdialog.ui \
    downloaddialog.ui \
    customizesplashdialog.ui
win32:RC_FILE += icon.rc





TRANSLATIONS = eepe_.ts    \
               eepe_he.ts  \
               eepe_pt.ts  \
               eepe_ru.ts  \
               eepe_de.ts  \
               eepe_es.ts  \
               eepe_fr.ts
