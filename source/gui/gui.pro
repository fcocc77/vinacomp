include(../../config/config.pro)

TEMPLATE = app
TARGET = "vinacomp"

INCLUDEPATH += $$files(*)
INCLUDEPATH += ../../libs/QCodeEditor/includes
INCLUDEPATH += ../../utils/c++

QT += core gui widgets multimedia

HEADERS +=  $$system(ls */*.h) \
            ../../utils/c++/*.h \
            ../../libs/QCodeEditor/includes/* \ 
            ../engine/python_api/python_api.h \
            ../engine/python_api/python.h \

SOURCES += 	 $$system(ls */*.cc) \
            ../../utils/c++/*.cc \
            ../../libs/QCodeEditor/src/* \
