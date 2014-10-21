#-------------------------------------------------
#
# Project created by QtCreator 2014-10-08T04:44:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Searcher
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
	indexLoader.cpp \
	searcher.cpp \
	coordinateIndexLoader.cpp \
	coordinateIndexSearcher.cpp

HEADERS += \
	indexLoader.h \
	searcher.h \
	coordinateIndexLoader.h \
	coordinateIndexSearcher.h
