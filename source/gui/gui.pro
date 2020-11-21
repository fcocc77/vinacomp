include(../../config/config.pro)

TEMPLATE = app
TARGET = "vinacomp"

INCLUDEPATH += $$files(*)
INCLUDEPATH += ../../libs/QCodeEditor/includes
INCLUDEPATH += ../../utils/c++/includes

QT += core gui widgets multimedia

HEADERS +=  $$system(ls */*.hpp) \
            ../../utils/c++/includes/* \
            ../../libs/QCodeEditor/includes/* 

SOURCES += 	 $$system(ls */*.cpp) \
            ../../utils/c++/src/* \
            ../../libs/QCodeEditor/src/* \
