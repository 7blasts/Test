<<<<<<< HEAD
#!include(gx_src_gdom.pri)
!contains (INCLUDEPATH,$$PWD){
  INCLUDEPATH+=$$PWD

  HEADERS    += $$PWD/gx_gdom.h
  HEADERS    += $$PWD/gx_gdom_json_dump.h
  HEADERS    += $$PWD/gx_gdom_print_fn.h
  HEADERS    += $$PWD/gx_gdom_type_name.hpp

  SOURCES    += $$PWD/gx_gdom_dg_disconnest.cpp
  SOURCES    += $$PWD/gx_gdom_dg_set_path.cpp
  SOURCES    += $$PWD/gx_gdom_ns_find.cpp
  SOURCES    += $$PWD/gx_gdom_fn.cpp
  SOURCES    += $$PWD/gx_gdom_fn_fn_bind.cpp
  SOURCES    += $$PWD/gx_gdom_fn_fn_calc.cpp
  SOURCES    += $$PWD/gx_gdom_invalidate.cpp
  SOURCES    += $$PWD/gx_gdom_path.cpp
  SOURCES    += $$PWD/gx_gdom_xx.cpp
}
=======
#!include(gx_src_gdom.pri)
!contains (INCLUDEPATH,$$PWD){
  INCLUDEPATH+=$$PWD

  HEADERS    += $$PWD/gx_gdom.h
  HEADERS    += $$PWD/gx_gdom_json_dump.h
  HEADERS    += $$PWD/gx_gdom_print_fn.h
  HEADERS    += $$PWD/gx_gdom_type_name.hpp

  SOURCES    += $$PWD/gx_gdom_dg_disconnest.cpp
  SOURCES    += $$PWD/gx_gdom_dg_set_path.cpp
  SOURCES    += $$PWD/gx_gdom_ns_find.cpp
  SOURCES    += $$PWD/gx_gdom_fn.cpp
  SOURCES    += $$PWD/gx_gdom_fn_fn_bind.cpp
  SOURCES    += $$PWD/gx_gdom_fn_fn_calc.cpp
  SOURCES    += $$PWD/gx_gdom_invalidate.cpp
  SOURCES    += $$PWD/gx_gdom_path.cpp
  SOURCES    += $$PWD/gx_gdom_xx.cpp
}
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182
