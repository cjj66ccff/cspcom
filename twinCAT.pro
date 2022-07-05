TARGET = twinCAT
TEMPLATE = lib
DEPENDPATH += .
INCLUDEPATH += .
DESTDIR = ../../modules
include(../../station.pri)
# Input
TRANSLATIONS += twinCAT_zh.ts \
    twinCAT_en.ts \

OTHER_FILES += twinCAT_zh.ts \
    twinCAT_en.ts \
    twinCAT.ini\
twinCAT.ver\

HEADERS += twinCATWdg.h \
    twinCATMod.h \
    twinCATFact.h \
    twinCATParam.h \

SOURCES += twinCATWdg.cpp \
    twinCATMod.cpp \
    twinCATFact.cpp \
    twinCATParam.cpp \

FORMS += twinCATWdg.ui

RESOURCES += twinCAT.qrc

