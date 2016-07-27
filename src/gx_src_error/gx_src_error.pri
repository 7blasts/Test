#!include(gx_src_error.pri)
!contains(INCLUDEPATH,$$PWD){
  HEADERS     += $$PWD/gx_error.h
  INCLUDEPATH += $$PWD
}
