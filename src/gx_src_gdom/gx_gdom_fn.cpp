<<<<<<< HEAD
#include "gx_gdom.h"

namespace _fn
{
	template<typename T> struct link: gx::dl
	{
		T **mp_ptr_ptr;

		void on(T *o) {
			(*mp_ptr_ptr) = o;
			success = true;
		}

		bool bind(void** pp, gx::xx* px)
		{
			mp_ptr_ptr = (T**)pp;
			px->on(this);
			return success;
		}
	};
	
	struct link_f1: _fn::link<gx::f1> { const char* src_type_name(){return "f1";} };
	struct link_f2: _fn::link<gx::f2> { const char* src_type_name(){return "f2";} };
	struct link_f3: _fn::link<gx::f3> { const char* src_type_name(){return "f3";} };
	struct link_f4: _fn::link<gx::f4> { const char* src_type_name(){return "f4";} };

	struct link_m2: _fn::link<gx::m2> { const char* src_type_name(){return "m2";} };
	struct link_m3: _fn::link<gx::m3> { const char* src_type_name(){return "m3";} };
	struct link_m4: _fn::link<gx::m4> { const char* src_type_name(){return "m4";} };

	struct link_b1: _fn::link<gx::b1> { const char* src_type_name(){return "b1";} };
	struct link_b2: _fn::link<gx::b2> { const char* src_type_name(){return "b2";} };
	struct link_b3: _fn::link<gx::b3> { const char* src_type_name(){return "b3";} };
	struct link_b4: _fn::link<gx::b4> { const char* src_type_name(){return "b4";} };

	struct link_i1: _fn::link<gx::i1> { const char* src_type_name(){return "i1";} };
	struct link_i2: _fn::link<gx::i2> { const char* src_type_name(){return "i2";} };
	struct link_i3: _fn::link<gx::i3> { const char* src_type_name(){return "i3";} };
	struct link_i4: _fn::link<gx::i4> { const char* src_type_name(){return "i4";} };

	struct link_ls: _fn::link<gx::ls> { const char* src_type_name(){return "ls";} };
	struct link_hr: _fn::link<gx::hr> { const char* src_type_name(){return "hr";} };
	struct link_s2: _fn::link<gx::s2> { const char* src_type_name(){return "s2";} };
	struct link_sc: _fn::link<gx::sc> { const char* src_type_name(){return "sc";} };
	struct link_sm: _fn::link<gx::sm> { const char* src_type_name(){return "sm";} };

	struct link_na: _fn::link<gx::na> { const char* src_type_name(){return "na";} };
	struct link_u0: _fn::link<gx::u0> { const char* src_type_name(){return "u0";} };

	struct link_ns: _fn::link<gx::ns> { const char* src_type_name(){return "ns";} };
}

// Add DG GET edges
gx::dg* gx::fn::get(gx::f1** pp, const char* name){static _fn::link_f1 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::f2** pp, const char* name){static _fn::link_f2 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::f3** pp, const char* name){static _fn::link_f3 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::f4** pp, const char* name){static _fn::link_f4 o; return get(&o,(void**)pp,name);}

gx::dg* gx::fn::get(gx::m2** pp, const char* name){static _fn::link_m2 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::m3** pp, const char* name){static _fn::link_m3 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::m4** pp, const char* name){static _fn::link_m4 o; return get(&o,(void**)pp,name);}

gx::dg* gx::fn::get(gx::b1** pp, const char* name){static _fn::link_b1 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::b2** pp, const char* name){static _fn::link_b2 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::b3** pp, const char* name){static _fn::link_b3 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::b4** pp, const char* name){static _fn::link_b4 o; return get(&o,(void**)pp,name);}

gx::dg* gx::fn::get(gx::i1** pp, const char* name){static _fn::link_i1 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::i2** pp, const char* name){static _fn::link_i2 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::i3** pp, const char* name){static _fn::link_i3 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::i4** pp, const char* name){static _fn::link_i4 o; return get(&o,(void**)pp,name);}

gx::dg* gx::fn::get(gx::ls** pp, const char* name){static _fn::link_ls o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::hr** pp, const char* name){static _fn::link_hr o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::s2** pp, const char* name){static _fn::link_s2 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::sc** pp, const char* name){static _fn::link_sc o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::sm** pp, const char* name){static _fn::link_sm o; return get(&o,(void**)pp,name);}

gx::dg* gx::fn::get(gx::na** pp, const char* name){static _fn::link_na o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::u0** pp, const char* name){static _fn::link_u0 o; return get(&o,(void**)pp,name);}

gx::dg* gx::fn::get(gx::ns** pp, const char* name){static _fn::link_ns o; return get(&o,(void**)pp,name);}

//Add DG SET edges
gx::dg* gx::fn::set(gx::f1** pp, const char* name){static _fn::link_f1 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::f2** pp, const char* name){static _fn::link_f2 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::f3** pp, const char* name){static _fn::link_f3 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::f4** pp, const char* name){static _fn::link_f4 o; return set(&o,(void**)pp,name);}

gx::dg* gx::fn::set(gx::m2** pp, const char* name){static _fn::link_m2 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::m3** pp, const char* name){static _fn::link_m3 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::m4** pp, const char* name){static _fn::link_m4 o; return set(&o,(void**)pp,name);}

gx::dg* gx::fn::set(gx::b1** pp, const char* name){static _fn::link_b1 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::b2** pp, const char* name){static _fn::link_b2 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::b3** pp, const char* name){static _fn::link_b3 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::b4** pp, const char* name){static _fn::link_b4 o; return set(&o,(void**)pp,name);}

gx::dg* gx::fn::set(gx::i1** pp, const char* name){static _fn::link_i1 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::i2** pp, const char* name){static _fn::link_i2 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::i3** pp, const char* name){static _fn::link_i3 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::i4** pp, const char* name){static _fn::link_i4 o; return set(&o,(void**)pp,name);}

gx::dg* gx::fn::set(gx::ls** pp, const char* name){static _fn::link_ls o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::hr** pp, const char* name){static _fn::link_hr o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::s2** pp, const char* name){static _fn::link_s2 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::sc** pp, const char* name){static _fn::link_sc o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::sm** pp, const char* name){static _fn::link_sm o; return set(&o,(void**)pp,name);}

gx::dg* gx::fn::set(gx::na** pp, const char* name){static _fn::link_na o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::u0** pp, const char* name){static _fn::link_u0 o; return set(&o,(void**)pp,name);}

gx::dg* gx::fn::set(gx::ns** pp, const char* name){static _fn::link_ns o; return set(&o,(void**)pp,name);}

bool gx::dg::bind()
{
	if(connected) // This mean: node in "connected state";
	{
		return true; 
	}

	if(NULL == node)  // This mean: node in "init state" or in "path-not-found";
	{
		node = path.find(code->mp_base); // success find are possible because situation in graph can be changed; 
		if(NULL == node)                 // Now is mean: "path-not-found"
		{
			(*from) = NULL;
			connected = false;  
			return false;                  //RETURN FALSE <connected:false><node:NULL>
		}
	}

	link->bind(from, node);            // Use stored linker

	if(link->success)
	{
		// register this edge in linked node
		connected = true;
		if(is_set_edge)
		{
			node->calc.insert(this);
			node->invalid = NEED_CALC;  // NEW SETTER CONNECTED! OLD VALUE ARE INCORRECT 
			node->invalidate();         // additional pass to parallel calcs
		}
		else //is get edge
		{
			node->deps.insert(this);
		}
		return true;
	}
	else
	{
		(*from) = NULL;
		connected = false;
		node = NULL;                     
	  return false;                    //RETURN FALSE <connected:false><node:NULL>
	}
}

gx::dg* gx::fn::get(gx::dl* link, void** pp, const char* name)
{
	gx::dg* edge = new gx::dg(this, link, pp, name);

	edge->is_set_edge = false;
	edge->path.set_path(name);

	edges_t::iterator subj = edges.find(name);

	if( subj == edges.end() )
	{
		edges[name] = edge;
	}
	else
	{
		// replace old dg edge:
		subj->second->disconnect();
		delete subj->second;
		subj->second = edge;
	}
	
	++get_lost_edges_count;
	
	return edge;
}

gx::dg* gx::fn::set(gx::dl* link, void** pp, const char* name)
{
	gx::dg* edge = new gx::dg(this, link, pp, name);

	edge->is_set_edge = true;
	edge->path.set_path(name);

	edges_t::iterator subj = edges.find(name);

	if( subj == edges.end() )
	{
		edges[name] = edge;
	}
	else
	{
		// replace old dg edge:
		subj->second->disconnect();
		delete subj->second;
		subj->second = edge;
	}
	
	++set_lost_edges_count;
	
	return edge;
}

gx::fn_builder_t gx::fn::builder(const char* name, gx::fn_builder_t func)
{
    static std::map<std::string, gx::fn_builder_t> dict;
    static std::map<std::string, gx::fn_builder_t>::iterator iter;
    if(func)
    {
        gx::fn_builder_t exists = NULL;
        iter = dict.find(name);
        if(dict.end() != iter){ exists = iter->second; }
        dict["name"] = func;
        return exists;
    }
    else
    {
        iter = dict.find(name);
        if(dict.end() != iter ) { return iter->second; }
        return NULL;
    }
}
=======
#include "gx_gdom.h"

namespace _fn
{
	template<typename T> struct link: gx::dl
	{
		T **mp_ptr_ptr;

		void on(T *o) {
			(*mp_ptr_ptr) = o;
			success = true;
		}

		bool bind(void** pp, gx::xx* px)
		{
			mp_ptr_ptr = (T**)pp;
			px->on(this);
			return success;
		}
	};
	
	struct link_f1: _fn::link<gx::f1> { const char* src_type_name(){return "f1";} };
	struct link_f2: _fn::link<gx::f2> { const char* src_type_name(){return "f2";} };
	struct link_f3: _fn::link<gx::f3> { const char* src_type_name(){return "f3";} };
	struct link_f4: _fn::link<gx::f4> { const char* src_type_name(){return "f4";} };

	struct link_m2: _fn::link<gx::m2> { const char* src_type_name(){return "m2";} };
	struct link_m3: _fn::link<gx::m3> { const char* src_type_name(){return "m3";} };
	struct link_m4: _fn::link<gx::m4> { const char* src_type_name(){return "m4";} };

	struct link_b1: _fn::link<gx::b1> { const char* src_type_name(){return "b1";} };
	struct link_b2: _fn::link<gx::b2> { const char* src_type_name(){return "b2";} };
	struct link_b3: _fn::link<gx::b3> { const char* src_type_name(){return "b3";} };
	struct link_b4: _fn::link<gx::b4> { const char* src_type_name(){return "b4";} };

	struct link_i1: _fn::link<gx::i1> { const char* src_type_name(){return "i1";} };
	struct link_i2: _fn::link<gx::i2> { const char* src_type_name(){return "i2";} };
	struct link_i3: _fn::link<gx::i3> { const char* src_type_name(){return "i3";} };
	struct link_i4: _fn::link<gx::i4> { const char* src_type_name(){return "i4";} };

	struct link_ls: _fn::link<gx::ls> { const char* src_type_name(){return "ls";} };
	struct link_hr: _fn::link<gx::hr> { const char* src_type_name(){return "hr";} };
	struct link_s2: _fn::link<gx::s2> { const char* src_type_name(){return "s2";} };
	struct link_sc: _fn::link<gx::sc> { const char* src_type_name(){return "sc";} };
	struct link_sm: _fn::link<gx::sm> { const char* src_type_name(){return "sm";} };

	struct link_na: _fn::link<gx::na> { const char* src_type_name(){return "na";} };
	struct link_u0: _fn::link<gx::u0> { const char* src_type_name(){return "u0";} };

	struct link_ns: _fn::link<gx::ns> { const char* src_type_name(){return "ns";} };
}

// Add DG GET edges
gx::dg* gx::fn::get(gx::f1** pp, const char* name){static _fn::link_f1 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::f2** pp, const char* name){static _fn::link_f2 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::f3** pp, const char* name){static _fn::link_f3 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::f4** pp, const char* name){static _fn::link_f4 o; return get(&o,(void**)pp,name);}

gx::dg* gx::fn::get(gx::m2** pp, const char* name){static _fn::link_m2 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::m3** pp, const char* name){static _fn::link_m3 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::m4** pp, const char* name){static _fn::link_m4 o; return get(&o,(void**)pp,name);}

gx::dg* gx::fn::get(gx::b1** pp, const char* name){static _fn::link_b1 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::b2** pp, const char* name){static _fn::link_b2 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::b3** pp, const char* name){static _fn::link_b3 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::b4** pp, const char* name){static _fn::link_b4 o; return get(&o,(void**)pp,name);}

gx::dg* gx::fn::get(gx::i1** pp, const char* name){static _fn::link_i1 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::i2** pp, const char* name){static _fn::link_i2 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::i3** pp, const char* name){static _fn::link_i3 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::i4** pp, const char* name){static _fn::link_i4 o; return get(&o,(void**)pp,name);}

gx::dg* gx::fn::get(gx::ls** pp, const char* name){static _fn::link_ls o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::hr** pp, const char* name){static _fn::link_hr o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::s2** pp, const char* name){static _fn::link_s2 o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::sc** pp, const char* name){static _fn::link_sc o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::sm** pp, const char* name){static _fn::link_sm o; return get(&o,(void**)pp,name);}

gx::dg* gx::fn::get(gx::na** pp, const char* name){static _fn::link_na o; return get(&o,(void**)pp,name);}
gx::dg* gx::fn::get(gx::u0** pp, const char* name){static _fn::link_u0 o; return get(&o,(void**)pp,name);}

gx::dg* gx::fn::get(gx::ns** pp, const char* name){static _fn::link_ns o; return get(&o,(void**)pp,name);}

//Add DG SET edges
gx::dg* gx::fn::set(gx::f1** pp, const char* name){static _fn::link_f1 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::f2** pp, const char* name){static _fn::link_f2 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::f3** pp, const char* name){static _fn::link_f3 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::f4** pp, const char* name){static _fn::link_f4 o; return set(&o,(void**)pp,name);}

gx::dg* gx::fn::set(gx::m2** pp, const char* name){static _fn::link_m2 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::m3** pp, const char* name){static _fn::link_m3 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::m4** pp, const char* name){static _fn::link_m4 o; return set(&o,(void**)pp,name);}

gx::dg* gx::fn::set(gx::b1** pp, const char* name){static _fn::link_b1 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::b2** pp, const char* name){static _fn::link_b2 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::b3** pp, const char* name){static _fn::link_b3 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::b4** pp, const char* name){static _fn::link_b4 o; return set(&o,(void**)pp,name);}

gx::dg* gx::fn::set(gx::i1** pp, const char* name){static _fn::link_i1 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::i2** pp, const char* name){static _fn::link_i2 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::i3** pp, const char* name){static _fn::link_i3 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::i4** pp, const char* name){static _fn::link_i4 o; return set(&o,(void**)pp,name);}

gx::dg* gx::fn::set(gx::ls** pp, const char* name){static _fn::link_ls o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::hr** pp, const char* name){static _fn::link_hr o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::s2** pp, const char* name){static _fn::link_s2 o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::sc** pp, const char* name){static _fn::link_sc o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::sm** pp, const char* name){static _fn::link_sm o; return set(&o,(void**)pp,name);}

gx::dg* gx::fn::set(gx::na** pp, const char* name){static _fn::link_na o; return set(&o,(void**)pp,name);}
gx::dg* gx::fn::set(gx::u0** pp, const char* name){static _fn::link_u0 o; return set(&o,(void**)pp,name);}

gx::dg* gx::fn::set(gx::ns** pp, const char* name){static _fn::link_ns o; return set(&o,(void**)pp,name);}

bool gx::dg::bind()
{
	if(connected) // This mean: node in "connected state";
	{
		return true; 
	}

	if(NULL == node)  // This mean: node in "init state" or in "path-not-found";
	{
		node = path.find(code->mp_base); // success find are possible because situation in graph can be changed; 
		if(NULL == node)                 // Now is mean: "path-not-found"
		{
			(*from) = NULL;
			connected = false;  
			return false;                  //RETURN FALSE <connected:false><node:NULL>
		}
	}

	link->bind(from, node);            // Use stored linker

	if(link->success)
	{
		// register this edge in linked node
		connected = true;
		if(is_set_edge)
		{
			node->calc.insert(this);
			node->invalid = NEED_CALC;  // NEW SETTER CONNECTED! OLD VALUE ARE INCORRECT 
			node->invalidate();         // additional pass to parallel calcs
		}
		else //is get edge
		{
			node->deps.insert(this);
		}
		return true;
	}
	else
	{
		(*from) = NULL;
		connected = false;
		node = NULL;                     
	  return false;                    //RETURN FALSE <connected:false><node:NULL>
	}
}

gx::dg* gx::fn::get(gx::dl* link, void** pp, const char* name)
{
	gx::dg* edge = new gx::dg(this, link, pp, name);

	edge->is_set_edge = false;
	edge->path.set_path(name);

	edges_t::iterator subj = edges.find(name);

	if( subj == edges.end() )
	{
		edges[name] = edge;
	}
	else
	{
		// replace old dg edge:
		subj->second->disconnect();
		delete subj->second;
		subj->second = edge;
	}
	
	++get_lost_edges_count;
	
	return edge;
}

gx::dg* gx::fn::set(gx::dl* link, void** pp, const char* name)
{
	gx::dg* edge = new gx::dg(this, link, pp, name);

	edge->is_set_edge = true;
	edge->path.set_path(name);

	edges_t::iterator subj = edges.find(name);

	if( subj == edges.end() )
	{
		edges[name] = edge;
	}
	else
	{
		// replace old dg edge:
		subj->second->disconnect();
		delete subj->second;
		subj->second = edge;
	}
	
	++set_lost_edges_count;
	
	return edge;
}

gx::fn_builder_t gx::fn::builder(const char* name, gx::fn_builder_t func)
{
    static std::map<std::string, gx::fn_builder_t> dict;
    static std::map<std::string, gx::fn_builder_t>::iterator iter;
    if(func)
    {
        gx::fn_builder_t exists = NULL;
        iter = dict.find(name);
        if(dict.end() != iter){ exists = iter->second; }
        dict["name"] = func;
        return exists;
    }
    else
    {
        iter = dict.find(name);
        if(dict.end() != iter ) { return iter->second; }
        return NULL;
    }
}
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182
