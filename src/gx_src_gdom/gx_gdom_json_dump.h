#ifndef GX_GDOM_JSON_DUMP_H
#define GX_GDOM_JSON_DUMP_H

#include "gx_gdom.h"

namespace gx
{
    // JSON serialiser
    struct gdom_json_dump: gx::xx::proc
		{
			  const char* tab;
        std::ostream *sp;
        std::string head;

        gdom_json_dump(std::ostream* sp, const char* head = NULL)
            : tab(" ")
						, sp(sp)
						, head( head ? head : "" )
        {}

        gdom_json_dump(std::ostream* sp, const std::string head)
            : tab(" ")
						, sp(sp)
						, head(head)
        {}

        void operator()(gx::xx* o)
				{
            if(o)
						{
                o->on(this);
            }
            else
						{
                (*sp)<<"gx::xx at <NULL> bad ptr!\n";
            }
        }

				void on_bv(gx::bv_t* v)
				{
					std::size_t i = 0;
					std::size_t last = v->size() - 1;
					for( ; i < v->size(); ++i)
					{
						(*sp) << v->at(i) << ((i==last) ? "":",");
					}
				}

				void on_iv(gx::iv_t* v)
				{
					std::size_t i = 0;
					std::size_t last = v->size() - 1;
					for( ; i < v->size(); ++i)
					{
						(*sp) << v->at(i) << ((i==last) ? "":",");
					}
				}

				void on_fv(gx::fv_t* v)
				{
					std::size_t i = 0;
					std::size_t last = v->size() - 1;
					for( ; i < v->size(); ++i)
					{
						(*sp) << v->at(i) << ((i==last) ? "":",");
					}
				}


        void on(gx::na*o){ (*sp) << "null at <" << o << ">\n"; }
				void on(gx::u0*o){
					(*sp) << head <<"{\":u0\":";
				(*sp) << "\"" << o->data << "\"";
				(*sp) << std::endl << head << "}";
				}
				void on(gx::f1*o){ (*sp) <<"{\":f1\":["; on_fv(&o->data); (*sp)<<"]}"; }
        void on(gx::f2*o){ (*sp) <<"{\":f2\":["; on_fv(&o->data); (*sp)<<"]}"; }
        void on(gx::f3*o){ (*sp) <<"{\":f3\":["; on_fv(&o->data); (*sp)<<"]}"; }
        void on(gx::f4*o){ (*sp) <<"{\":f4\":["; on_fv(&o->data); (*sp)<<"]}"; }
        void on(gx::m2*o){ (*sp) <<"{\":m2\":["; on_fv(&o->data); (*sp)<<"]}"; }
        void on(gx::m3*o){ (*sp) <<"{\":m3\":["; on_fv(&o->data); (*sp)<<"]}"; }
        void on(gx::m4*o){ (*sp) <<"{\":m4\":["; on_fv(&o->data); (*sp)<<"]}"; }
        void on(gx::b1*o){ (*sp) <<"{\":b1\":["; on_bv(&o->data); (*sp)<<"]}"; }
        void on(gx::b2*o){ (*sp) <<"{\":b2\":["; on_bv(&o->data); (*sp)<<"]}"; }
        void on(gx::b3*o){ (*sp) <<"{\":b3\":["; on_bv(&o->data); (*sp)<<"]}"; }
        void on(gx::b4*o){ (*sp) <<"{\":b4\":["; on_bv(&o->data); (*sp)<<"]}"; }
				void on(gx::i1*o){ (*sp) <<"{\":i1\":["; on_iv(&o->data); (*sp)<<"]}"; }
        void on(gx::i2*o){ (*sp) <<"{\":i2\":["; on_iv(&o->data); (*sp)<<"]}"; }
        void on(gx::i3*o){ (*sp) <<"{\":i3\":["; on_iv(&o->data); (*sp)<<"]}"; }
        void on(gx::i4*o){ (*sp) <<"{\":i4\":["; on_iv(&o->data); (*sp)<<"]}"; }
        void on(gx::s2*o){ (*sp) <<"sampler2d at <" << o << ">" << std::endl; }
        void on(gx::sc*o){ (*sp) <<"samplerCube at <" << o << ">" << std::endl; }
				void on(gx::ns*o){
					// here file name with type extention not supported *.f1.js, *.u0.js, ... ,
					// So, here make only contents of "unknown_name.ns.js"
					if(o->data.size())
					{
					  (*sp)<<"\n"<< head << "{\":ns\":";
            (*sp)<<"\n"<< head << tab <<"{"; 
						gx::ns::data_t::iterator iter=o->data.begin(), stop=o->data.end(), last=--o->data.end();
						std::string tabs = head + tab + tab;
						gx::gdom_json_dump i_dumps(sp, tabs); i_dumps.tab=tab;
						for(;iter != stop; ++iter){
							(*sp) << "\"" << iter->first <<"\":"; // "\n"
							i_dumps(iter->second);
							if(iter!=last)
							{
								(*sp )<<"\n"<<head<<tab<<","; // <not last>";
							}
						}
					  (*sp) << "\n"<< head <<"}}";
					}
					else
					{
						(*sp)<<"{\":ns\":{}}"; 
					}
				}
        void on(gx::ls*o){ (*sp) << "gx-list at <" << o << ">" << std::endl; }
        void on(gx::hr*o){ (*sp) << "gx-link at <" << o << ">" << std::endl; }
        void on(gx::sm*o){ (*sp) << "gx-state-machine at <" << o << ">" << std::endl; }

    }; // struct dumps_t
} //namespace gx

#endif // GX_GDOM_JSON_DUMP_H
