#
# Third-party: 'glm' http://glm.g-truc.net/0.9.7/index.html
#
# ... OpenGL Mathematics (GLM) ... 
# ... the Happy Bunny License (Modified MIT) or the MIT License. 
#
# USAGE: include (../third_party_src_glm/third_party_src_glm.pri)
#
#!include(third_party_src_glm.pri)

!contains ( INCLUDEPATH, $$PWD ){
  INCLUDEPATH += $$PWD
}
