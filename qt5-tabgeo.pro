#-------------------------------------------------
#
# Project created by QtCreator 2015-07-15T16:02:52
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = qt5-tabgeo
CONFIG   += console
CONFIG   += C++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    country.cpp

HEADERS += \
    country.h

DISTFILES += \
    tabgeo_country_v4.dat

