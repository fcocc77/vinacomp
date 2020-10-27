include(../config/config.pro)

TEMPLATE = app
TARGET = "vinacomp"

INCLUDEPATH += ./includes
INCLUDEPATH += ./includes/knob
INCLUDEPATH += ../libs/QCodeEditor/includes
INCLUDEPATH += ../utils/c++/includes

QT += core gui widgets multimedia

HEADERS +=  ./includes/*.hpp \
            ./includes/knob/* \
            ../utils/c++/includes/* \
            ../libs/QCodeEditor/includes/* 

SOURCES += 	./src/*.cpp \
            ./src/knob/* \
            ../utils/c++/src/* \
            ../libs/QCodeEditor/src/* \
