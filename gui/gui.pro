include(../config/config.pro)

TEMPLATE = app
TARGET = "vinacomp"

INCLUDES = ./includes

INCLUDEPATH += $$INCLUDES

QT += core gui widgets multimedia

HEADERS +=  $$INCLUDES/* 

SOURCES += 	./src/* 
