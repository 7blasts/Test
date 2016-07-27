#!include(gx_src_gdom_glx.pri)
!contains(INCLUDEPATH,$$PWD){
  INCLUDEPATH+=$$PWD

  HEADERS    += $$PWD/gx_glx_gdom.h
  HEADERS    += $$PWD/gx_glx_abstract_drawer.h

  SOURCES    += $$PWD/gx_gl_camera.cpp

  include(../gx_src_gdom/gx_src_gdom.pri)
  include(../gx_src_gdom_glm/gx_src_gdom_glm.pri)
}
