include(../../config/config.pro)

TEMPLATE = app
TARGET = "vinaengine"

CONFIG += console
QT += core widgets multimedia


INCLUDEPATH += $$files(*)
INCLUDEPATH += ../../utils/c++

HEADERS +=  $$system(ls */*.h) \
            ../../utils/c++/*.h \

SOURCES += 	 $$system(ls */*.cc) \
            ../../utils/c++/*.cc \