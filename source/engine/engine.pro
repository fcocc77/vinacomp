include(../../config/config.pro)

TEMPLATE = app
TARGET = "vinaengine"

CONFIG += console
QT += core widgets multimedia

DEFINES += "ENGINE" 

INCLUDEPATH += $$files(*)
INCLUDEPATH += ../../utils/c++
INCLUDEPATH += ../python_api

HEADERS +=  $$system(ls */*.h) \
            ../../utils/c++/*.h \
            ../python_api/*.h \

SOURCES += 	 $$system(ls */*.cc) \
            ../../utils/c++/*.cc \
            ../python_api/*.cc \