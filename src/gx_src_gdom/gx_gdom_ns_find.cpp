<<<<<<< HEAD
#include <gx_gdom.h>

gx::f1* gx::ns::find_f1(const char*n){static is_it_type<gx::f1> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::f1*)px : NULL;}
gx::f2* gx::ns::find_f2(const char*n){static is_it_type<gx::f2> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::f2*)px : NULL;}
gx::f3* gx::ns::find_f3(const char*n){static is_it_type<gx::f3> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::f3*)px : NULL;}
gx::f4* gx::ns::find_f4(const char*n){static is_it_type<gx::f4> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::f4*)px : NULL;}

gx::m2* gx::ns::find_m2(const char*n){static is_it_type<gx::m2> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::m2*)px : NULL;}
gx::m3* gx::ns::find_m3(const char*n){static is_it_type<gx::m3> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::m3*)px : NULL;}
gx::m4* gx::ns::find_m4(const char*n){static is_it_type<gx::m4> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::m4*)px : NULL;}

gx::b1* gx::ns::find_b1(const char*n){static is_it_type<gx::b1> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::b1*)px : NULL;}
gx::b2* gx::ns::find_b2(const char*n){static is_it_type<gx::b2> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::b2*)px : NULL;}
gx::b3* gx::ns::find_b3(const char*n){static is_it_type<gx::b3> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::b3*)px : NULL;}
gx::b4* gx::ns::find_b4(const char*n){static is_it_type<gx::b4> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::b4*)px : NULL;}

gx::i1* gx::ns::find_i1(const char*n){static is_it_type<gx::i1> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::i1*)px : NULL;}
gx::i2* gx::ns::find_i2(const char*n){static is_it_type<gx::i2> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::i2*)px : NULL;}
gx::i3* gx::ns::find_i3(const char*n){static is_it_type<gx::i3> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::i3*)px : NULL;}
gx::i4* gx::ns::find_i4(const char*n){static is_it_type<gx::i4> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::i4*)px : NULL;}

gx::ls* gx::ns::find_ls(const char*n){static is_it_type<gx::ls> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::ls*)px : NULL;}
gx::hr* gx::ns::find_hr(const char*n){static is_it_type<gx::hr> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::hr*)px : NULL;}
gx::s2* gx::ns::find_s2(const char*n){static is_it_type<gx::s2> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::s2*)px : NULL;}
gx::sc* gx::ns::find_sc(const char*n){static is_it_type<gx::sc> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::sc*)px : NULL;}
gx::sm* gx::ns::find_sm(const char*n){static is_it_type<gx::sm> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::sm*)px : NULL;}

gx::ns* gx::ns::find_ns(const char*n){static is_it_type<gx::ns> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::ns*)px : NULL;}

=======
#include <gx_gdom.h>

gx::f1* gx::ns::find_f1(const char*n){static is_it_type<gx::f1> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::f1*)px : NULL;}
gx::f2* gx::ns::find_f2(const char*n){static is_it_type<gx::f2> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::f2*)px : NULL;}
gx::f3* gx::ns::find_f3(const char*n){static is_it_type<gx::f3> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::f3*)px : NULL;}
gx::f4* gx::ns::find_f4(const char*n){static is_it_type<gx::f4> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::f4*)px : NULL;}

gx::m2* gx::ns::find_m2(const char*n){static is_it_type<gx::m2> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::m2*)px : NULL;}
gx::m3* gx::ns::find_m3(const char*n){static is_it_type<gx::m3> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::m3*)px : NULL;}
gx::m4* gx::ns::find_m4(const char*n){static is_it_type<gx::m4> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::m4*)px : NULL;}

gx::b1* gx::ns::find_b1(const char*n){static is_it_type<gx::b1> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::b1*)px : NULL;}
gx::b2* gx::ns::find_b2(const char*n){static is_it_type<gx::b2> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::b2*)px : NULL;}
gx::b3* gx::ns::find_b3(const char*n){static is_it_type<gx::b3> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::b3*)px : NULL;}
gx::b4* gx::ns::find_b4(const char*n){static is_it_type<gx::b4> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::b4*)px : NULL;}

gx::i1* gx::ns::find_i1(const char*n){static is_it_type<gx::i1> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::i1*)px : NULL;}
gx::i2* gx::ns::find_i2(const char*n){static is_it_type<gx::i2> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::i2*)px : NULL;}
gx::i3* gx::ns::find_i3(const char*n){static is_it_type<gx::i3> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::i3*)px : NULL;}
gx::i4* gx::ns::find_i4(const char*n){static is_it_type<gx::i4> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::i4*)px : NULL;}

gx::ls* gx::ns::find_ls(const char*n){static is_it_type<gx::ls> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::ls*)px : NULL;}
gx::hr* gx::ns::find_hr(const char*n){static is_it_type<gx::hr> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::hr*)px : NULL;}
gx::s2* gx::ns::find_s2(const char*n){static is_it_type<gx::s2> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::s2*)px : NULL;}
gx::sc* gx::ns::find_sc(const char*n){static is_it_type<gx::sc> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::sc*)px : NULL;}
gx::sm* gx::ns::find_sm(const char*n){static is_it_type<gx::sm> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::sm*)px : NULL;}

gx::ns* gx::ns::find_ns(const char*n){static is_it_type<gx::ns> good; gx::xx* px=find_xx(n); return(px && good(px)) ? (gx::ns*)px : NULL;}

>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182
