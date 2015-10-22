QT += core network
QT -= gui

TARGET = chat-server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    chatserver.cpp

HEADERS += \
    chatserver.h

