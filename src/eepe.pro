HEADERS       = mainwindow.h \
                mdichild.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                mdichild.cpp
RESOURCES     = eepe.qrc

# install
target.path = eepe
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS eepe.pro images
sources.path = eepe
INSTALLS += target sources

