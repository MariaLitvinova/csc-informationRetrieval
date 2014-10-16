#-------------------------------------------------
#
# Project created by QtCreator 2014-10-07T20:56:10
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Indexer
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
	stemmer.cpp \
	utils.cpp \
	indexer.cpp \
	coordinateIndexer.cpp

HEADERS += \
	stemmer.h \
	utils.h \
	indexer.h \
	coordinateIndexer.h
