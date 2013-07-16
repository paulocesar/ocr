#-------------------------------------------------
#
# Project created by QtCreator 2013-07-15T15:31:34
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = OCR
CONFIG   += console static
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ocrserver.cpp \
    ocrserverhandler.cpp \
    ocr.cpp \
    ocrdocument.cpp \
    ocrexception.cpp \
    ocrlog.cpp

HEADERS += \
    ocrserver.h \
    ocrserverhandler.h \
    ocr.h \
    ocrdocument.h \
    ocrexception.h \
    ocrlog.h
