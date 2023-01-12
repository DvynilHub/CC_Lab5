QT += core concurrent widgets

CONFIG += c++11

TARGET = multiplication_of_numbers
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
mywidget.cpp

DEFINES += QT_DEPRECATED_WARNINGS
HEADERS += \
mywidget.h
