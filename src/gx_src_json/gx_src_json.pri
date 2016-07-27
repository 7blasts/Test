#!include(gx_src_json.pri)
!contains ( INCLUDEPATH, $$PWD ){
  HEADERS     += $$PWD/gx_json.h

  SOURCES     += $$PWD/lib/gx_json_str.cpp
  SOURCES     += $$PWD/lib/gx_json_free.cpp

  INCLUDEPATH += $$PWD

  include (../gx_src_error/gx_src_error.pri)
  include (../third_party_src_libjson/third_party_src_libjson.pri)
}
