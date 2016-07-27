#ifndef GX_GDOM_TYPE_NAME_H
#define GX_GDOM_TYPE_NAME_H

#include "gx_gdom.h"

namespace gx
{
	struct gdom_type_name: gx::xx::const_proc
	{
		const char* mp_type_name;

		const char* operator()(const gx::f1&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::f2&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::f3&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::f4&o){on(&o); return mp_type_name;}

		const char* operator()(const gx::m2&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::m3&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::m4&o){on(&o); return mp_type_name;}

		const char* operator()(const gx::b1&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::b2&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::b3&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::b4&o){on(&o); return mp_type_name;}

		const char* operator()(const gx::i1&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::i2&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::i3&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::i4&o){on(&o); return mp_type_name;}

		const char* operator()(const gx::ls&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::hr&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::s2&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::sc&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::sm&o){on(&o); return mp_type_name;}

		const char* operator()(const gx::na&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::u0&o){on(&o); return mp_type_name;}
		const char* operator()(const gx::ns&o){on(&o); return mp_type_name;}

		const char* operator()(gx::xx*o){o->with_const_proc(this); return mp_type_name;}

        void on(const gx::f1*){static const char* tid="f1"; mp_type_name = tid;}
        void on(const gx::f2*){static const char* tid="f2"; mp_type_name = tid;}
        void on(const gx::f3*){static const char* tid="f3"; mp_type_name = tid;}
        void on(const gx::f4*){static const char* tid="f4"; mp_type_name = tid;}

        void on(const gx::m2*){static const char* tid="m2"; mp_type_name = tid;}
        void on(const gx::m3*){static const char* tid="m3"; mp_type_name = tid;}
        void on(const gx::m4*){static const char* tid="m4"; mp_type_name = tid;}

        void on(const gx::b1*){static const char* tid="b1"; mp_type_name = tid;}
        void on(const gx::b2*){static const char* tid="b2"; mp_type_name = tid;}
        void on(const gx::b3*){static const char* tid="b3"; mp_type_name = tid;}
        void on(const gx::b4*){static const char* tid="b4"; mp_type_name = tid;}

        void on(const gx::i1*){static const char* tid="i1"; mp_type_name = tid;}
        void on(const gx::i2*){static const char* tid="i2"; mp_type_name = tid;}
        void on(const gx::i3*){static const char* tid="i3"; mp_type_name = tid;}
        void on(const gx::i4*){static const char* tid="i4"; mp_type_name = tid;}

        void on(const gx::ls*){static const char* tid="ls"; mp_type_name = tid;}
        void on(const gx::hr*){static const char* tid="hr"; mp_type_name = tid;}
        void on(const gx::s2*){static const char* tid="s2"; mp_type_name = tid;}
        void on(const gx::sc*){static const char* tid="sc"; mp_type_name = tid;}
        void on(const gx::sm*){static const char* tid="sm"; mp_type_name = tid;}

        void on(const gx::na*){static const char* tid="na"; mp_type_name = tid;}
        void on(const gx::u0*){static const char* tid="u0"; mp_type_name = tid;}
        void on(const gx::ns*){static const char* tid="ns"; mp_type_name = tid;}
	};
}

#endif  // GX_GDOM_TYPE_NAME_H
