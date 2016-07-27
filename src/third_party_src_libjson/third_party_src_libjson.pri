#
# Third-party: 'libjson' http://sourceforge.net/projects/libjson/
#
# ... JSON reader and writer ... 
#
# Here used only as reader, adopted for use as source files, w/o .lib
#
# USAGE: include (../third_party_src_libjson/third_party_src_libjson.pri)
#
#!include(third_party_src_libjson.pri)
!contains ( SOURCES, $$PWD/src/json_reader.cpp ){
  HEADERS += \
    $$PWD/include/json/autolink.h \
    $$PWD/include/json/config.h \
    $$PWD/include/json/features.h \
    $$PWD/include/json/forwards.h \
    $$PWD/include/json/json.h \
    $$PWD/include/json/reader.h \
    $$PWD/include/json/value.h \
    $$PWD/include/json/writer.h \
    $$PWD/src/json_batchallocator.h
  SOURCES += \
    $$PWD/src/json_reader.cpp \
    $$PWD/src/json_value.cpp \
    $$PWD/src/json_writer.cpp \
    $$PWD/src/json_internalarray.inl \
    $$PWD/src/json_internalmap.inl  \
    $$PWD/src/json_valueiterator.inl
  DEPENDPATH  += $$PWD/src
  INCLUDEPATH += $$PWD/include
}
