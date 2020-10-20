include(../config/config.pro)

TEMPLATE = app
TARGET = "vinacomp"

INCLUDES = ./includes

INCLUDEPATH += $$INCLUDES
INCLUDEPATH += ../utils/c++/includes

QT += core gui widgets multimedia

HEADERS +=  $$INCLUDES/* \
            ../utils/c++/includes/* \

SOURCES += 	./src/* \
            ../utils/c++/src/* \
