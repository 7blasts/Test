// test_nodes_2.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>

#include "gx_gdom_print_fn.h"
#include "gx_gdom_type_name.hpp"
#include "gx_gdom_json_dump.h"

using namespace std;

gx::ns* gx::root()
{
	static struct values_tree {
		gx::ns* root;
		values_tree() {
			printf("++root at <0x%p>\n", this );
			printf("  before create root\n", this );
			root = new gx::ns;
			printf("  root created at <0x%p>\n", root );
		}
		~values_tree() {
			printf("--root at <0x%p>\n", this );
			printf("  before delete root at <0x%p>\n", root );
			printf("  after root deleted...\n", root );
		}
	}o; return o.root;
}

int _tmain(int argc, _TCHAR* argv[])
{
	/***type_name_tests BEGIN;
	gx::gdom_type_name tid; // type_name getter

	gx::f1* p_f1 = new gx::f1(); cout<< "f1 " << tid(p_f1) << " " << tid(*p_f1) << endl;
  gx::f2* p_f2 = new gx::f2(); cout<< "f2 " << tid(p_f2) << " " << tid(*p_f2) << endl;
  gx::f3* p_f3 = new gx::f3(); cout<< "f3 " << tid(p_f3) << " " << tid(*p_f3) << endl;
  gx::f4* p_f4 = new gx::f4(); cout<< "f4 " << tid(p_f4) << " " << tid(*p_f4) << endl;

  gx::m2* p_m2 = new gx::m2(); cout<< "m2 " << tid(p_m2) << " " << tid(*p_m2) << endl;
  gx::m3* p_m3 = new gx::m3(); cout<< "m3 " << tid(p_m3) << " " << tid(*p_m3) << endl;
  gx::m4* p_m4 = new gx::m4(); cout<< "m4 " << tid(p_m4) << " " << tid(*p_m4) << endl;

	gx::ns* p_ns = new gx::ns(); cout<< "ns " << tid(p_ns) << " " << tid(*p_ns) << endl;
	*///END


	gx::root()->data["g_f1"] = new gx::f1; gx::root()->data["s_f1"] = new gx::f1;
	gx::root()->data["g_f2"] = new gx::f2; gx::root()->data["s_f2"] = new gx::f2;
	gx::root()->data["g_f3"] = new gx::f3; gx::root()->data["s_f3"] = new gx::f3;
	gx::root()->data["g_f4"] = new gx::f4; gx::root()->data["s_f4"] = new gx::f4;

	gx::root()->data["g_m2"] = new gx::m2; gx::root()->data["s_m2"] = new gx::m2;
	gx::root()->data["g_m3"] = new gx::m3; gx::root()->data["s_m3"] = new gx::m3;
	gx::root()->data["g_m4"] = new gx::m4; gx::root()->data["s_m4"] = new gx::m4;

	gx::root()->data["g_b1"] = new gx::b1; gx::root()->data["s_b1"] = new gx::b1;
	gx::root()->data["g_b2"] = new gx::b2; gx::root()->data["s_b2"] = new gx::b2;
	gx::root()->data["g_b3"] = new gx::b3; gx::root()->data["s_b3"] = new gx::b3;
	gx::root()->data["g_b4"] = new gx::b4; gx::root()->data["s_b4"] = new gx::b4;

	gx::root()->data["g_i1"] = new gx::i1; gx::root()->data["s_i1"] = new gx::i1;
	gx::root()->data["g_i2"] = new gx::i2; gx::root()->data["s_i2"] = new gx::i2;
	gx::root()->data["g_i3"] = new gx::i3; gx::root()->data["s_i3"] = new gx::i3;
	gx::root()->data["g_i4"] = new gx::i4; gx::root()->data["s_i4"] = new gx::i4;

	gx::root()->data["g_ns"] = new gx::ns; gx::root()->data["s_ns"] = new gx::ns;

	gx::gdom_json_dump dumps(&std::cout,"<head>"); dumps.tab = "<tab>";

	std::cout<<"ns json dump:";	dumps(gx::root()); std::cout<<"\n";

	struct test_fn_t: gx::fn
	{
		gx::f1 *g_f1_ptr, *s_f1_ptr; // fv float get/set
		gx::f2 *g_f2_ptr, *s_f2_ptr;
		gx::f3 *g_f3_ptr, *s_f3_ptr;
		gx::f4 *g_f4_ptr, *s_f4_ptr;

		gx::m2 *g_m2_ptr, *s_m2_ptr; // fv matrices get/set
		gx::m3 *g_m3_ptr, *s_m3_ptr;
		gx::m4 *g_m4_ptr, *s_m4_ptr;

		gx::b1 *g_b1_ptr, *s_b1_ptr; //fv bool
		gx::b2 *g_b2_ptr, *s_b2_ptr;
		gx::b3 *g_b3_ptr, *s_b3_ptr;
		gx::b4 *g_b4_ptr, *s_b4_ptr;

		gx::i1 *g_i1_ptr, *s_i1_ptr; //fv long
		gx::i2 *g_i2_ptr, *s_i2_ptr;
		gx::i3 *g_i3_ptr, *s_i3_ptr;
		gx::i4 *g_i4_ptr, *s_i4_ptr;

		gx::ns *g_ns_ptr, *s_ns_ptr;

		test_fn_t(gx::ns* base): fn(base)
		{
			printf("++test_fn_t at <0x%p>\n", this);
			// create direct bind to input values
			
			//=======================<add GET-EDGES>
			get(&g_f1_ptr, "g_f1");
			get(&g_f2_ptr, "g_f2");
			get(&g_f3_ptr, "g_f3");
			get(&g_f4_ptr, "g_f4");

			get(&g_m2_ptr, "g_m2");
			get(&g_m3_ptr, "g_m3");
			get(&g_m4_ptr, "g_m4");

			get(&g_b1_ptr, "g_b1");
			get(&g_b2_ptr, "g_b2");
			get(&g_b3_ptr, "g_b3");
			get(&g_b4_ptr, "g_b4");

			get(&g_i1_ptr, "g_i1");
			get(&g_i2_ptr, "g_i2");
			get(&g_i3_ptr, "g_i3");
			get(&g_i4_ptr, "g_i4");

			get(&g_ns_ptr, "g_ns");
      
			//=======================<add SET-EDGES> 
			set(&s_f1_ptr, "s_f1");
			set(&s_f2_ptr, "s_f2");
			set(&s_f3_ptr, "s_f3");
			set(&s_f4_ptr, "s_f4");

			set(&s_m2_ptr, "s_m2");
			set(&s_m3_ptr, "s_m3");
			set(&s_m4_ptr, "s_m4");

			set(&s_b1_ptr, "s_b1");
			set(&s_b2_ptr, "s_b2");
			set(&s_b3_ptr, "s_b3");
			set(&s_b4_ptr, "s_b4");

			set(&s_i1_ptr, "s_i1");
			set(&s_i2_ptr, "s_i2");
			set(&s_i3_ptr, "s_i3");
			set(&s_i4_ptr, "s_i4");

			set(&s_ns_ptr, "s_ns");
		}
		virtual~test_fn_t(){
			printf("--test_fn_t at <0x%p>\n", this);
		}
		void calc__()
		{
			// never called from DG, because set_dict is empty
			// if this minimal wrapper change self "get" field,
			// invalidate from getted field must call by self.
			// It is not some valid way to do changes in DG.
		}
		void calc_emulation()
		{
			printf(     " - min_fn_t at <0x%p> BEGIN\n", this );
			if(fn_bind()){
				printf(   " - min_fn_t at <0x%p> >>bind success\n", this );
				if(fn_calc())
				{
					printf( " - min_fn_t at <0x%p> >>>>deps success\n", this );
          // can calc and invalidate self output!
				}
				else
				{
					printf( " - min_fn_t at <0x%p> >>>>deps FAILED\n", this );
				}
			}
			else
			{
				printf(   " - min_fn_t at <0x%p> >>bind FAILED\n", this );
			}
			printf(     " - min_fn_t at <0x%p> END\n", this );
		}
	};

	// (FAILED) Supported strategy: (FAILED)
	//   1. If old fn with equal code-name exists in base ns, overwrite old;
	//      manage old fn ptr as master - stack him or destroy - now old fn is slave.
	//      Possible detect old-fn's-set_dict's edges and and rebind all of them to
	//      other output fields; (NEEDED for insert calc's wrapper on fly (at run time)
	//   2. If fn with equal type exists in base ns, overwrite his output nodes by setting self
	//      as node's gx::fn* xx::calc; now new instance is master of the output nodes, old is
	//      his slave. 
	//   3. All fn, and groups of fn, disigned as states of some gigant cascaded state machine.

	// Supported strategy:
	//   1. Multiple CALCs can be assigned to single GDOM node, is needed if some part
	//      of GDOM node controlled by parallel independed writers. It's very usefull
	//      when developer do not want change or wrap old, successfully tested code
	//      when add additional functionality. And also in case of parallel co-routines
	//      calculated different parts of GDOM array from different sources and with
	//      different speed.
	//   2. To change some node's INVALID state to "0", INVALID-state "0" must be in
	//      all edges in std::set<gx::dg*> gx::xx::calc;
	//   3. Invalidate call all virtual fn::on_invalidate(gx::dg*, unsigned short WHAT)
	//      On each invalidate, by default, must paint self to INVALID, but INVALID why?;
	//        NEED_CALC = 1 - need calc - traditional for DG
	//        DEEP_FAIL = 4 - in self deps, at least one calc has SELF_FAIL flag
	//        SELF_FAIL = 8 - has error log or some way to extract error log
	//   4. Life time of the "code": -ns-base destroy all self "code" in destructor
	//        ns-base|-------------------|
	//        "code1" |-------|         |
	//          "max" |-----------------|
	//          "min" |....|............|
	//   5. Life time of the "edge": -fn-self destroy all self "edges" in destructor
	//        ns-base|-------------------|


	// Create instanse of "test_fn_t", using "gx::root()" as calc base:
	test_fn_t* test_fn_ptr = new test_fn_t( gx::root() );

	// При наследовании:
	// struct C от B, B от A, A от virtual gx::fn — все конструкторы будут вызваны в одном
	// вызове new C(gx::ns* nest), и управление в однозадачный DG не вернётся, пока вся
	// цепочка конструкторов не выполнится:
	//   gx::fn::fn(nest)
	//   A::A(nest)  { all 'A' get/set attributes setup }  // if(fn_bind()) => level "A" interface supported
	//   B::B(nest)  { all 'B' get/set attributes setup }  // if(fn_bind()) => level "B" interface supported
	//   C::C(nest)  { all 'C' get/set attributes setup }  // if(fn_bind()) => level "C" interface supported
	// test call of "fn_bind" в конструкторе may be useful to detect base ns's interface level.

	// freshly created fn instance
	gx::fn_print_info(std::cout, test_fn_ptr, "<before bind>");

	// first call
	test_fn_ptr->calc_emulation();

	// fn- state after first call:
	gx::fn_print_info(std::cout, test_fn_ptr, "<after bind>");

	// redirect attribute path
	//  - if binded, increment lost edges
	//  - if unbinded, check lost edges != 0;
	
	gx::dg* s_f1_edge = test_fn_ptr->find_edge("s_f1");
	if(s_f1_edge)
	{
		s_f1_edge -> set_path ("bla.bla.bla");
	}

	gx::fn_print_info(std::cout, test_fn_ptr, "<after set_path>");
	
	delete test_fn_ptr;

	return 0;
}

