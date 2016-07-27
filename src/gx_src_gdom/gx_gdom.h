#ifndef GX_GDOM_H
#define GX_GDOM_H

#include <set>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <sstream>

#define NEED_CALC 1   // We real need it for DG computational model push-pull-paint

#define DEEP_FAIL 4   // possible useful on visualisation DG ( track in graph with red edges to failed deps )
#define SELF_FAIL 8   // possible useful on visualisation DG

namespace gx
{
    // GDOM Numeric groups
    typedef std::vector<float> fv_t;  // float buffer    f1 f2 f3 f4 m2 m3 m4
    typedef std::vector<bool>  bv_t;  // boolean buffer  b1 b2 b3 b4
    typedef std::vector<long>  iv_t;  // integer buffer  i1 i2 i3 i4

    // GDOM visited data types
    struct u0; struct na;	struct ns; // ascii-string, none, dict
    struct f1; struct f2; struct f3; struct f4; struct m2; struct m3; struct m4;  // visited fv-based;
    struct b1; struct b2; struct b3; struct b4;  // visited bv-based
    struct i1; struct i2; struct i3; struct i4;  // visited iv-based
    struct hr; struct ls; struct s2; struct sc; struct sm;  // visited futures (reserved)

    ns* root(); //gdom values tree root

    // GDOM DG base 
    //
    struct fn; // gdom DG code plug; stored only in ns: std::map<std::string, gx::fn*> code;
    struct dg; // gdom DG dependency; stored only in fn input and output maps;

    typedef fn* (*fn_builder_t)(ns*, const char*);   // declare builder function ptr

    // GDOM visited (abstract base class)
    struct xx
    {
        unsigned short invalid;
        void invalidate(const unsigned short& flags = 0);

        gx::ns* nest;  // parent gdom namespace (folder like folder fhere this file exists)

        std::set<gx::ns*> uses;  // data refs to this item "hr"
        std::set<gx::dg*> calc;  // calculator, code used this gdom node as output
        std::set<gx::dg*> deps;  // code refs to this item

        bool no_refs()  // no needed as input => no refs, 
        {
            return   uses.size()   ?  // false if has data refs
                false: deps.size()   ?  // false if has code refs
                false:(NULL == nest) ?  // false if has no nest (possible auto life time)
                false: true;            // true in other case
        }

        struct proc;                      // abstract visitor;
        struct const_proc;                // abstract const visitor;
        struct is_it_type_base_abstract;  // type checker's base empty struct;

        virtual void on(proc*)=0;
        virtual void with_const_proc(const_proc*)=0;

        static void on_kill(gx::xx*);

        explicit xx(gx::ns* nest = NULL)
            :invalid(0)  // no calc - mean no deps, no way to be invalid;
            ,nest(nest)  // NULL mean it is independed object w/o parent;
        {
        }

        virtual~xx()
        {
        }
    };

    // GDOM path, where char "." is path separator, like (name1.name2.name3)
    struct path
    {
        typedef std::list<std::string> data_t;
        std::list<std::string> data;
        bool has_invalid_syntax(){ return false; }
        std::ostringstream err;

        explicit path(const char* p="");

        path(const std::string&);

        void set_path(const std::string& src); // change path to new one

        gx::xx* find(gx::ns*); // find gdom node use this path
        // starts from dict used as find first argument
        // => return NULL if path not exists in gdom
        //    if NULL returned, use err.str() to get last error
        std::string subpath(const std::size_t& last);
        std::string repr();    // return path as std::string
        std::string str(){return repr();}    // return repr()
        std::string info();
        std::string get_last_find_error(){return err.str();}
    protected:
        void parse(const std::string& src);
    };

    // GDOM visitor (abstract base class)
    struct xx::proc
    {
        virtual void on(gx::b1*)=0;  // std::vector<bool>  with min-size and step 1;
        virtual void on(gx::b2*)=0;  // std::vector<bool>  with min-size and step 2;
        virtual void on(gx::b3*)=0;  // std::vector<bool>  with min-size and step 3;
        virtual void on(gx::b4*)=0;  // std::vector<bool>  with min-size and step 4;

        virtual void on(gx::i1*)=0;  // std::vector<long>  with min-size and step 1;
        virtual void on(gx::i2*)=0;  // std::vector<long>  with min-size and step 2;
        virtual void on(gx::i3*)=0;  // std::vector<long>  with min-size and step 3;
        virtual void on(gx::i4*)=0;  // std::vector<long>  with min-size and step 4;

        virtual void on(gx::f1*)=0;  // std::vector<float> with min-size and step 1;
        virtual void on(gx::f2*)=0;  // std::vector<float> with min-size and step 2;
        virtual void on(gx::f3*)=0;  // std::vector<float> with min-size and step 3;
        virtual void on(gx::f4*)=0;  // std::vector<float> with min-size and step 4;

        virtual void on(gx::m2*)=0;  // std::vector<float> with min-size and step 4;
        virtual void on(gx::m3*)=0;  // std::vector<float> with min-size and step 9;
        virtual void on(gx::m4*)=0;  // std::vector<float> with min-size and step 16;
        virtual void on(gx::u0*)=0;  // std::string data;  // ASCII
        virtual void on(gx::na*)=0;  // n\a, None, empty;
        virtual void on(gx::ns*)=0;  // std::map<std::string, gx::xx*> data;  // GDOM
        // std::map<std::string, gx::fn*> code;  // DG

        virtual void on(gx::hr*)=0;  // future reserved;
        virtual void on(gx::ls*)=0;  // future reserved;
        virtual void on(gx::s2*)=0;  // future reserved;
        virtual void on(gx::sc*)=0;  // future reserved;
        virtual void on(gx::sm*)=0;  // future reserved;

        virtual~proc(){}
    };

    // GDOM const visitor (abstract base class)
    struct xx::const_proc
    {
        virtual void on(const gx::b1*)=0;  // std::vector<bool>  with min-size and step 1;
        virtual void on(const gx::b2*)=0;  // std::vector<bool>  with min-size and step 2;
        virtual void on(const gx::b3*)=0;  // std::vector<bool>  with min-size and step 3;
        virtual void on(const gx::b4*)=0;  // std::vector<bool>  with min-size and step 4;

        virtual void on(const gx::i1*)=0;  // std::vector<long>  with min-size and step 1;
        virtual void on(const gx::i2*)=0;  // std::vector<long>  with min-size and step 2;
        virtual void on(const gx::i3*)=0;  // std::vector<long>  with min-size and step 3;
        virtual void on(const gx::i4*)=0;  // std::vector<long>  with min-size and step 4;

        virtual void on(const gx::f1*)=0;  // std::vector<float> with min-size and step 1;
        virtual void on(const gx::f2*)=0;  // std::vector<float> with min-size and step 2;
        virtual void on(const gx::f3*)=0;  // std::vector<float> with min-size and step 3;
        virtual void on(const gx::f4*)=0;  // std::vector<float> with min-size and step 4;

        virtual void on(const gx::m2*)=0;  // std::vector<float> with min-size and step 4;
        virtual void on(const gx::m3*)=0;  // std::vector<float> with min-size and step 9;
        virtual void on(const gx::m4*)=0;  // std::vector<float> with min-size and step 16;
        virtual void on(const gx::u0*)=0;  // std::string data;  // ASCII
        virtual void on(const gx::na*)=0;  // n\a, None, empty;
        virtual void on(const gx::ns*)=0;  // std::map<std::string, gx::xx*> data;  // GDOM
        // std::map<std::string, gx::fn*> code;  // DG

        virtual void on(const gx::hr*)=0;  // future reserved;
        virtual void on(const gx::ls*)=0;  // future reserved;
        virtual void on(const gx::s2*)=0;  // future reserved;
        virtual void on(const gx::sc*)=0;  // future reserved;
        virtual void on(const gx::sm*)=0;  // future reserved;

        virtual~const_proc(){}
    };

    // minimal type-detector's base class based on const xx visitor
    struct xx::is_it_type_base_abstract: xx::const_proc
    {
        bool itis;

        virtual	void on(const gx::b1*){itis=false;}  // std::vector<bool>  with min-size and step 1;
        virtual void on(const gx::b2*){itis=false;}  // std::vector<bool>  with min-size and step 2;
        virtual void on(const gx::b3*){itis=false;}  // std::vector<bool>  with min-size and step 3;
        virtual void on(const gx::b4*){itis=false;}  // std::vector<bool>  with min-size and step 4;

        virtual void on(const gx::i1*){itis=false;}  // std::vector<long>  with min-size and step 1;
        virtual void on(const gx::i2*){itis=false;}  // std::vector<long>  with min-size and step 2;
        virtual void on(const gx::i3*){itis=false;}  // std::vector<long>  with min-size and step 3;
        virtual void on(const gx::i4*){itis=false;}  // std::vector<long>  with min-size and step 4;

        virtual void on(const gx::f1*){itis=false;}  // std::vector<float> with min-size and step 1;
        virtual void on(const gx::f2*){itis=false;}  // std::vector<float> with min-size and step 2;
        virtual void on(const gx::f3*){itis=false;}  // std::vector<float> with min-size and step 3;
        virtual void on(const gx::f4*){itis=false;}  // std::vector<float> with min-size and step 4;

        virtual void on(const gx::m2*){itis=false;}  // std::vector<float> with min-size and step 4;
        virtual void on(const gx::m3*){itis=false;}  // std::vector<float> with min-size and step 9;
        virtual void on(const gx::m4*){itis=false;}  // std::vector<float> with min-size and step 16;
        virtual void on(const gx::u0*){itis=false;}  // std::string data;  // ASCII
        virtual void on(const gx::na*){itis=false;}  // n\a, None, empty;
        virtual void on(const gx::ns*){itis=false;}  // std::map<std::string, gx::xx*> data;  // GDOM
        //                                           // std::map<std::string, gx::fn*> code;  // DG
        virtual void on(const gx::hr*){itis=false;}  // future reserved;
        virtual void on(const gx::ls*){itis=false;}  // future reserved;
        virtual void on(const gx::s2*){itis=false;}  // future reserved;
        virtual void on(const gx::sc*){itis=false;}  // future reserved;
        virtual void on(const gx::sm*){itis=false;}  // future reserved;

        bool operator()(gx::xx* o) { o->with_const_proc(this); return itis; }

        virtual~is_it_type_base_abstract(){}
    };

    // xx based classes type detector, usage:
    // void foo(gx::xx* xx_ptr){ 
    //   static is_it_type<gx::f1> is_f1_ptr; // create checker
    //   if (is_f1_ptr(xx_ptr))               // use checker
    //   {
    //      gx_f1* f1_ptr = (gx::f1*)xx_ptr;  // here type conversion are safety
    //      ...
    //   }
    // }
    template <typename T>	struct is_it_type: gx::xx::is_it_type_base_abstract
    {
        void on(T*){ itis=true; }
    };

    struct dl: gx::xx::proc  // abstract gdom-data-to-fn-attribute linker
    {
        bool success;
        virtual const char* src_type_name()=0;
        virtual bool bind(void**, gx::xx*)=0;
        const char* dst_type_name(){return type_name;}

    protected:
        const char* type_name;

        virtual void on(gx::f1*){success=false; type_name = "f1";}
        virtual void on(gx::f2*){success=false; type_name = "f2";}
        virtual void on(gx::f3*){success=false; type_name = "f3";}
        virtual void on(gx::f4*){success=false; type_name = "f4";}

        virtual void on(gx::m2*){success=false; type_name = "m2";}
        virtual void on(gx::m3*){success=false; type_name = "m3";}
        virtual void on(gx::m4*){success=false; type_name = "m4";}

        virtual void on(gx::b1*){success=false; type_name = "b1";}
        virtual void on(gx::b2*){success=false; type_name = "b2";}
        virtual void on(gx::b3*){success=false; type_name = "b3";}
        virtual void on(gx::b4*){success=false; type_name = "b4";}

        virtual void on(gx::i1*){success=false; type_name = "i1";}
        virtual void on(gx::i2*){success=false; type_name = "i2";}
        virtual void on(gx::i3*){success=false; type_name = "i3";}
        virtual void on(gx::i4*){success=false; type_name = "i4";}

        virtual void on(gx::ls*){success=false; type_name = "ls";}
        virtual void on(gx::hr*){success=false; type_name = "hr";}
        virtual void on(gx::s2*){success=false; type_name = "s2";}
        virtual void on(gx::sc*){success=false; type_name = "sc";}
        virtual void on(gx::sm*){success=false; type_name = "sm";}

        virtual void on(gx::na*){success=false; type_name = "na";}
        virtual void on(gx::u0*){success=false; type_name = "u0";}
        virtual void on(gx::ns*){success=false; type_name = "ns";}

        virtual ~dl(){}
    };
}

struct gx::f1: gx::xx{~f1(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::fv_t data; f1(const std::size_t sz=1):data(sz*1){}};
struct gx::f2: gx::xx{~f2(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::fv_t data; f2(const std::size_t sz=1):data(sz*2){}};
struct gx::f3: gx::xx{~f3(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::fv_t data; f3(const std::size_t sz=1):data(sz*3){}};
struct gx::f4: gx::xx{~f4(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::fv_t data; f4(const std::size_t sz=1):data(sz*4){}};

struct gx::m2: gx::xx{~m2(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::fv_t data; m2(const std::size_t sz=1):data(sz*4){}};
struct gx::m3: gx::xx{~m3(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::fv_t data; m3(const std::size_t sz=1):data(sz*9){}};
struct gx::m4: gx::xx{~m4(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::fv_t data; m4(const std::size_t sz=1):data(sz*16){}};

struct gx::b1: gx::xx{~b1(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::bv_t data; b1(const std::size_t sz=1):data(sz*1){}};
struct gx::b2: gx::xx{~b2(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::bv_t data; b2(const std::size_t sz=1):data(sz*2){}};
struct gx::b3: gx::xx{~b3(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::bv_t data; b3(const std::size_t sz=1):data(sz*3){}};
struct gx::b4: gx::xx{~b4(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::bv_t data; b4(const std::size_t sz=1):data(sz*4){}};

struct gx::i1: gx::xx{~i1(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::iv_t data; i1(const std::size_t sz=1):data(sz*1){}};
struct gx::i2: gx::xx{~i2(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::iv_t data; i2(const std::size_t sz=1):data(sz*2){}};
struct gx::i3: gx::xx{~i3(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::iv_t data; i3(const std::size_t sz=1):data(sz*3){}};
struct gx::i4: gx::xx{~i4(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} gx::iv_t data; i4(const std::size_t sz=1):data(sz*4){}};

struct gx::ls: gx::xx{~ls(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} };
struct gx::hr: gx::xx{~hr(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} };
struct gx::s2: gx::xx{~s2(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} };
struct gx::sc: gx::xx{~sc(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} };
struct gx::sm: gx::xx{~sm(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} };

struct gx::na: gx::xx{~na(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);}};

struct gx::u0: gx::xx{~u0(){ xx::on_kill(this); }; void with_const_proc(gx::xx::const_proc*o){o->on(this);} void on(gx::xx::proc*o){o->on(this);} std::string data;};

struct gx::ns: gx::xx
{
    void with_const_proc(gx::xx::const_proc*o){o->on(this);}  // xx::const_proc visitors support
    void on(gx::xx::proc*o){o->on(this);}          // xx::proc visitors support
    typedef std::map<std::string, gx::xx*> data_t; // data dict type
    typedef std::map<std::string, gx::fn*> code_t; // code dict type
    std::map<std::string, gx::xx*> data;           // dict of the gdom "xx" pointers
    std::map<std::string, gx::fn*> code;           // dict of the code "fn" pointers

    fn* make_fn(const char*);
    fn* make_fn(fn_builder_t);

    //f1* make_f1(const char* name, std::size_t sz = 1);
    //f2* make_f2(const char* name, std::size_t sz = 1);
    //f3* make_f3(const char* name, std::size_t sz = 1);
    //f4* make_f4(const char* name, std::size_t sz = 1);
    

    inline gx::fn* find_fn(const char*n) { code_t::iterator subj=code.find(n); return subj==code.end() ? NULL : subj->second; }
    inline gx::xx* find_xx(const char*n) { data_t::iterator subj=data.find(n); return subj==data.end() ? NULL : subj->second; }

    // xx based classes type detector, usage:
    // void foo(gx::xx* xx_ptr){ 
    //   static is_it_type<gx::f1> is_f1_ptr; // create checker
    //   if (is_f1_ptr(xx_ptr))               // use checker
    //   {
    //      gx_f1* f1_ptr = (gx::f1*)xx_ptr;  // here type conversion are safety
    //      ...
    //   }
    // }

    gx::f1* find_f1(const char*);
    gx::f2* find_f2(const char*);
    gx::f3* find_f3(const char*);
    gx::f4* find_f4(const char*);

    gx::m2* find_m2(const char*);
    gx::m3* find_m3(const char*);
    gx::m4* find_m4(const char*);

    gx::b1* find_b1(const char*);
    gx::b2* find_b2(const char*);
    gx::b3* find_b3(const char*);
    gx::b4* find_b4(const char*);

    gx::i1* find_i1(const char*);
    gx::i2* find_i2(const char*);
    gx::i3* find_i3(const char*);
    gx::i4* find_i4(const char*);

    gx::ls* find_ls(const char*);
    gx::hr* find_hr(const char*);
    gx::s2* find_s2(const char*);
    gx::sc* find_sc(const char*);
    gx::sm* find_sm(const char*);

    gx::ns* find_ns(const char*);

    ~ns(){ xx::on_kill(this); };
};

struct gx::dg  // concrete type, storage of the dynamic data link info
{
    // typedef unsigned short error_t;
    // static const error_t find_error_bit=1;  // field not found
    // static const error_t type_error_bit=2;  // bind type error
    // static const error_t set_error_bit=4;   // write out put has writer

    // error_t find(const std::string&);

    // ":fn": {"gx.gl.proj":{"fov":"gx.gl.default.fov", "test":"?...scene.fov"} }
    // loader begin work with this field only after all other fields are loaded

    // ":m4":"gx.path.to.ns.with.my.calculator.instance.code.all_tail_symbols_is_code_type_name"
    bool connected;               // connected state. Setted to false by deleted or replaced gdom instance
    bool is_set_edge;             // contain true if it is a output dg edge, else false
    std::string name;             // fn::set / fn::get dg-factory ( set linker and name )
    gx::path path;                // string path from nest to xx
    gx::fn* code;                 // is can't be NULL, setted in constructor ( self code object )
    gx::dl* link;                 // is can't be NULL, setted in constructor ( self linker object )
    void ** from;                 // = NULL;                                 ( self target )
    gx::xx* node;                 // target node, initially NULL, setted up by path.find(code->mp_base); 

    friend struct gx::fn;         // fn::set / fn::get dg-factory ( set linker and name )
    friend struct gx::xx;

private:

    dg(gx::fn* code, gx::dl* link, void **from, const char* name)
        :connected(false)
        ,name(name)
        ,path("")   // setted from gx::fn
        ,code(code)
        ,link(link)
        ,from(from)
        ,node(NULL)
    {
    }

public:

    gx::dg& set_path(const char*);  // change edge's target path ( WARNING: accept even wrong path )
    bool bind();                    // direct connect fn's pointer to node at target path, return NULL on fail;
    void disconnect();              // disconnect fn's pointer from node at target path;
};

struct gx::fn
{
    unsigned short invalid;  // DG-push-pull-painting-invalid model's flag
    unsigned short aid;      // aspect's ID

    gx::ns* mp_base;

    std::size_t get_lost_edges_count;
    std::size_t set_lost_edges_count;


    // bool connected(){ return get_lost_edges_count ? false: set_lost_edges_count ? false: true; }

    typedef std::map<std::string, gx::dg*>	edges_t;

    edges_t edges; //set_dict;

    gx::dg* get(gx::f1** pp, const char* name);
    gx::dg* set(gx::f1** pp, const char* name);
    gx::dg* get(gx::f2** pp, const char* name);
    gx::dg* set(gx::f2** pp, const char* name);
    gx::dg* get(gx::f3** pp, const char* name);
    gx::dg* set(gx::f3** pp, const char* name);
    gx::dg* get(gx::f4** pp, const char* name);
    gx::dg* set(gx::f4** pp, const char* name);

    gx::dg* get(gx::m2** pp, const char* name);
    gx::dg* set(gx::m2** pp, const char* name);
    gx::dg* get(gx::m3** pp, const char* name);
    gx::dg* set(gx::m3** pp, const char* name);
    gx::dg* get(gx::m4** pp, const char* name);
    gx::dg* set(gx::m4** pp, const char* name);

    gx::dg* get(gx::b1** pp, const char* name);
    gx::dg* set(gx::b1** pp, const char* name);
    gx::dg* get(gx::b2** pp, const char* name);
    gx::dg* set(gx::b2** pp, const char* name);
    gx::dg* get(gx::b3** pp, const char* name);
    gx::dg* set(gx::b3** pp, const char* name);
    gx::dg* get(gx::b4** pp, const char* name);
    gx::dg* set(gx::b4** pp, const char* name);

    gx::dg* get(gx::i1** pp, const char* name);
    gx::dg* set(gx::i1** pp, const char* name);
    gx::dg* get(gx::i2** pp, const char* name);
    gx::dg* set(gx::i2** pp, const char* name);
    gx::dg* get(gx::i3** pp, const char* name);
    gx::dg* set(gx::i3** pp, const char* name);
    gx::dg* get(gx::i4** pp, const char* name);
    gx::dg* set(gx::i4** pp, const char* name);

    gx::dg* get(gx::ls** pp, const char* name);
    gx::dg* set(gx::ls** pp, const char* name);
    gx::dg* get(gx::hr** pp, const char* name);
    gx::dg* set(gx::hr** pp, const char* name);
    gx::dg* get(gx::s2** pp, const char* name);
    gx::dg* set(gx::s2** pp, const char* name);
    gx::dg* get(gx::sc** pp, const char* name);
    gx::dg* set(gx::sc** pp, const char* name);
    gx::dg* get(gx::sm** pp, const char* name);
    gx::dg* set(gx::sm** pp, const char* name);

    gx::dg* get(gx::na** pp, const char* name);
    gx::dg* set(gx::na** pp, const char* name);
    gx::dg* get(gx::u0** pp, const char* name);
    gx::dg* set(gx::u0** pp, const char* name);
    gx::dg* get(gx::ns** pp, const char* name);
    gx::dg* set(gx::ns** pp, const char* name);

    bool fn_bind();  // try bind all dependences, skip all binded
    bool fn_calc();  // try calc all dependences, skip all calculated

    gx::dg* find_edge(const std::string &name)
    {
        edges_t::iterator subj = edges.find(name);
        return ( subj == edges.end() ) ? NULL : subj->second;
    }

    void invalidate(const unsigned short& flags = 0);
    void calc_success_no_changes() { invalid = 0; }   // nothing todo on no changes
    void calc_success(){ calc_success_no_changes(); invalidate();} // todo on after write changes to output
    void fn_bind_failed(){}
    void fn_calc_failed(){}

    virtual void calc()=0;

    fn(gx::ns* base)
        :invalid(NEED_CALC)
        ,aid(0)
        ,mp_base(base)
        ,get_lost_edges_count(0)
        ,set_lost_edges_count(0)
    {
    }

    virtual~fn()
    {
        for(edges_t::iterator curr = edges.begin(), stop = edges.end(); curr != stop; ++curr)
        {
            if(curr->second)
            {
                if(curr->second->connected)
                {
                    curr->second->disconnect();
                }
                delete curr->second;
            }
        }
    }

    static fn_builder_t builder(const char*, fn_builder_t = NULL);   // get builder or register new one
    //                                                               // fn_builder_t old = builder("name", new_one)

protected:
    gx::dg* set( gx::dl* link, void **pp, const char* name );  // create and add to edges new output node
    gx::dg* get( gx::dl* link, void **pp, const char* name );  // create and add to edges new input node
};

#endif  // GX_GDOM_H
