<<<<<<< HEAD
#include <iostream>

#include "gx_gdom.h"
#include "gx_gdom_type_name.hpp"

using namespace std;

void gx::xx::on_kill(gx::xx* o)
{
	static struct: gx::xx::proc
	{
		void on(gx::f1*o){ cout << "--+f1" << " at <0x" << o << ">" << endl; }
		void on(gx::f2*o){ cout << "--+f2" << " at <0x" << o << ">" << endl; }
		void on(gx::f3*o){ cout << "--+f3" << " at <0x" << o << ">" << endl; }
		void on(gx::f4*o){ cout << "--+f4" << " at <0x" << o << ">" << endl; }

		void on(gx::m2*o){ cout << "--+m2" << " at <0x" << o << ">" << endl; }
		void on(gx::m3*o){ cout << "--+m3" << " at <0x" << o << ">" << endl; }
		void on(gx::m4*o){ cout << "--+m4" << " at <0x" << o << ">" << endl; }

		void on(gx::b1*o){ cout << "--+b1" << " at <0x" << o << ">" << endl; }
		void on(gx::b2*o){ cout << "--+b2" << " at <0x" << o << ">" << endl; }
		void on(gx::b3*o){ cout << "--+b3" << " at <0x" << o << ">" << endl; }
		void on(gx::b4*o){ cout << "--+b4" << " at <0x" << o << ">" << endl; }

		void on(gx::i1*o){ cout << "--+i1" << " at <0x" << o << ">" << endl; }
		void on(gx::i2*o){ cout << "--+i2" << " at <0x" << o << ">" << endl; }
		void on(gx::i3*o){ cout << "--+i3" << " at <0x" << o << ">" << endl; }
		void on(gx::i4*o){ cout << "--+i4" << " at <0x" << o << ">" << endl; }

		void on(gx::ls*o){ cout << "--+ls" << " at <0x" << o << ">" << endl; }
		void on(gx::hr*o){ cout << "--+hr" << " at <0x" << o << ">" << endl; }
		void on(gx::s2*o){ cout << "--+s2" << " at <0x" << o << ">" << endl; }
		void on(gx::sc*o){ cout << "--+sc" << " at <0x" << o << ">" << endl; }
		void on(gx::sm*o){ cout << "--+sm" << " at <0x" << o << ">" << endl; }

		void on(gx::u0*o){ cout << "--+u0" << " at <0x" << o << ">" << endl; }
		void on(gx::na*o){ cout << "--+na" << " at <0x" << o << ">" << endl; }
		void on(gx::ns*o){ cout << "--+ns" << " at <0x" << o << ">" << endl; }
	}
	last_with_only; o->on(&last_with_only);
}
=======
#include <iostream>

#include "gx_gdom.h"
#include "gx_gdom_type_name.hpp"

using namespace std;

void gx::xx::on_kill(gx::xx* o)
{
	static struct: gx::xx::proc
	{
		void on(gx::f1*o){ cout << "--+f1" << " at <0x" << o << ">" << endl; }
		void on(gx::f2*o){ cout << "--+f2" << " at <0x" << o << ">" << endl; }
		void on(gx::f3*o){ cout << "--+f3" << " at <0x" << o << ">" << endl; }
		void on(gx::f4*o){ cout << "--+f4" << " at <0x" << o << ">" << endl; }

		void on(gx::m2*o){ cout << "--+m2" << " at <0x" << o << ">" << endl; }
		void on(gx::m3*o){ cout << "--+m3" << " at <0x" << o << ">" << endl; }
		void on(gx::m4*o){ cout << "--+m4" << " at <0x" << o << ">" << endl; }

		void on(gx::b1*o){ cout << "--+b1" << " at <0x" << o << ">" << endl; }
		void on(gx::b2*o){ cout << "--+b2" << " at <0x" << o << ">" << endl; }
		void on(gx::b3*o){ cout << "--+b3" << " at <0x" << o << ">" << endl; }
		void on(gx::b4*o){ cout << "--+b4" << " at <0x" << o << ">" << endl; }

		void on(gx::i1*o){ cout << "--+i1" << " at <0x" << o << ">" << endl; }
		void on(gx::i2*o){ cout << "--+i2" << " at <0x" << o << ">" << endl; }
		void on(gx::i3*o){ cout << "--+i3" << " at <0x" << o << ">" << endl; }
		void on(gx::i4*o){ cout << "--+i4" << " at <0x" << o << ">" << endl; }

		void on(gx::ls*o){ cout << "--+ls" << " at <0x" << o << ">" << endl; }
		void on(gx::hr*o){ cout << "--+hr" << " at <0x" << o << ">" << endl; }
		void on(gx::s2*o){ cout << "--+s2" << " at <0x" << o << ">" << endl; }
		void on(gx::sc*o){ cout << "--+sc" << " at <0x" << o << ">" << endl; }
		void on(gx::sm*o){ cout << "--+sm" << " at <0x" << o << ">" << endl; }

		void on(gx::u0*o){ cout << "--+u0" << " at <0x" << o << ">" << endl; }
		void on(gx::na*o){ cout << "--+na" << " at <0x" << o << ">" << endl; }
		void on(gx::ns*o){ cout << "--+ns" << " at <0x" << o << ">" << endl; }
	}
	last_with_only; o->on(&last_with_only);
}
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182
