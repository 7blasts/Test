QT += core
QT += network
QT -= gui

#CONFIG += c++11

TARGET = howto_qudpsocket_sync
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

include (../gx_src_json/gx_src_json.pri)
