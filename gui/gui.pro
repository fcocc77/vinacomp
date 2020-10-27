include(../config/config.pro)

TEMPLATE = app
TARGET = "vinacomp"

INCLUDES = ./includes

INCLUDEPATH += $$INCLUDES 
INCLUDEPATH += ../libs/QCodeEditor/includes
INCLUDEPATH += ../utils/c++/includes

QT += core gui widgets multimedia

HEADERS +=  $$INCLUDES/* \
            ../utils/c++/includes/* \
            ../libs/QCodeEditor/includes/* 

SOURCES += 	./src/* \
            ../utils/c++/src/* \
            ../libs/QCodeEditor/src/* \
