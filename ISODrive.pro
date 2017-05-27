TARGET = ISODrive

CONFIG += sailfishapp

SOURCES += src/ISODrive.cpp \
    src/filemanager.cpp \
    src/isomanager.cpp

OTHER_FILES += qml/ISODrive.qml \
    qml/cover/CoverPage.qml \
    rpm/ISODrive.changes.in \
    rpm/ISODrive.spec \
    rpm/ISODrive.yaml \
    translations/*.ts \
    ISODrive.desktop \
    qml/pages/MainPage.qml

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/ISODrive-de.ts \
    translations/ISODrive-ru.ts

HEADERS += \
    src/filemanager.h \
    src/isomanager.h

