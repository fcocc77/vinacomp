include(../config/config.pro)

TEMPLATE = app
TARGET = "vinacomp"

INCLUDES = ./includes
CODE_EDITOR = ../libs/QCodeEditor/include

INCLUDEPATH += $$CODE_EDITOR 
INCLUDEPATH += $$INCLUDES
INCLUDEPATH += ../utils/c++/includes
INCLUDEPATH += ../libs/QCodeEditor/includes

QT += core gui widgets multimedia

HEADERS +=  $$INCLUDES/* \
            ../utils/c++/includes/* \
            $$CODE_EDITOR/KGL/Design/* \
			$$CODE_EDITOR/KGL/Widgets/* \
			$$CODE_EDITOR/KGL/KGLConfig.hpp \

SOURCES += 	./src/* \
            ../utils/c++/src/* \
            ../libs/QCodeEditor/src/Design/*.cpp \
			../libs/QCodeEditor/src/Widgets/*.cpp \
