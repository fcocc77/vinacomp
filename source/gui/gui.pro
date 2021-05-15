include(../../config/config.pro)

TEMPLATE = app
TARGET = "vinacomp"

INCLUDEPATH += $$files(*)
INCLUDEPATH += ../../libs/QCodeEditor/includes
INCLUDEPATH += ../../utils/c++
INCLUDEPATH += ../python_api
INCLUDEPATH += ../ofx
INCLUDEPATH += ../global
INCLUDEPATH += ../engine/render
INCLUDEPATH += ../engine/project
INCLUDEPATH += ../engine/nodes

# ofx libreria
INCLUDEPATH += ../../libs/ofx/include
#

QT += core gui widgets multimedia

DEFINES += "GUI"

HEADERS +=  $$system(ls */*.h) \
            ../../utils/c++/*.h \
            ../../libs/QCodeEditor/includes/* \
            ../../libs/ofx/include/*.h \
            ../python_api/*.h \
            ../ofx/*.h \
            ../global/*.h \
			../engine/render/*.h \
			../engine/project/*.h \
			../engine/nodes/*.h \

SOURCES += 	 $$system(ls */*.cc) \
            ../../utils/c++/*.cc \
            ../../libs/QCodeEditor/src/* \
            ../python_api/*.cc \
            ../ofx/*.cc \
			../engine/render/*.cc \
			../engine/project/*.cc \
			../engine/nodes/*.cc \
