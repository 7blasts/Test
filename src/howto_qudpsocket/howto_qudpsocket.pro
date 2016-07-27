QT += core
QT += network
QT -= gui

#CONFIG += c++11

TARGET = howto_qudpsocket
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
HEADERS += gx_udp.hpp
HEADERS += gx_udp_qt_driver.h

