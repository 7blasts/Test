#!include(gx_src_gdom_glm.pri)
!contains(INCLUDEPATH,$$PWD){
  INCLUDEPATH += $$PWD

  HEADERS     += $$PWD/gx_glm_utils.h

  include (../gx_src_gdom/gx_src_gdom.pri)
  include (../third_party_src_glm/third_party_src_glm.pri)
}

