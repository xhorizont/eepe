HEADERS       += mainwindow.h \
                mdichild.h   \
				file.h       \
				pers.h       \
				myeeprom.h   
SOURCES       += main.cpp \
                mainwindow.cpp \
                mdichild.cpp   \
				file.cpp       \
				pers.cpp
RESOURCES     += eepe.qrc

# install
#target.path = eepe
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS eepe.pro images
#sources.path = eepe
#INSTALLS += target sources

TARGET = eepe
TEMPLATE = app

