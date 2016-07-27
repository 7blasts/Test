QT += core
QT -= gui

#CONFIG += c++11

TARGET = gx_exe_test_0000
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

include (../third_party_src_libjson/third_party_src_libjson.pri) 
include (../gx_src_error/gx_src_error.pri)

include (../gx_src_json/gx_src_json.pri)
include (../gx_src_gdom/gx_src_gdom.pri)
