#ifndef GX_JSON_WRAPPER_H_INCLUDED
#define GX_JSON_WRAPPER_H_INCLUDED

/*
Visitable JSON. Not some QT5 JSON, not needed MOC (metaobject compilator).
So no way to support QML. But used third lib really fast and support line
comments in JSON.
*/

#include <stdexcept>
#include <sstream>
#include <string>
#include <list>
#include <map>

#include <gx_error.h>
//#include <gx_json.h>

namespace gx{
    struct json;
    struct syntax: std::logic_error { //GDOM-from-Json loads syntax error
        syntax(const std::string& s) :std::logic_error(s){}
    };
    json* from(const char*);
}

#define GX_JSON_NONE 0
#define GX_JSON_DICT 1
#define GX_JSON_LIST 2
#define GX_JSON_CSTR 3
#define GX_JSON_SINT 4
#define GX_JSON_UINT 5
#define GX_JSON_REAL 6
#define GX_JSON_BOOL 7

struct gx::json{
	struct DICT;   // JSON => objectValue
	struct LIST;   // JSON => arrayValue
	struct NONE;   // JSON => NULL
	struct CSTR;   // JSON => STRING
	struct SINT;   // JSON => SIGNED INT
	struct UINT;   // JSON => UNSIGNED INT
	struct REAL;   // JSON => REAL
	struct BOOL;   // JSON => BOOL
	struct proc{   // Abstract Visitor (visitor pattern)
		virtual void on(gx::json::DICT*)=0;
		virtual void on(gx::json::LIST*)=0;
		virtual void on(gx::json::NONE*)=0;
		virtual void on(gx::json::CSTR*)=0;
		virtual void on(gx::json::SINT*)=0;
		virtual void on(gx::json::UINT*)=0;
		virtual void on(gx::json::REAL*)=0;
		virtual void on(gx::json::BOOL*)=0;
		virtual~proc(){}
	};
    struct type_guard;
    struct type_name;
    struct type_id;
	static void free(gx::json*);
	virtual~json(){}
	virtual void on(gx::json::proc*)=0; /*visitor support*/
	virtual bool is_dict(){ return false; }
	virtual bool is_list(){ return false; }
	virtual bool is_null(){ return false; }
	virtual bool is_cstr(){ return false; }
	virtual bool is_sint(){ return false; }
	virtual bool is_uint(){ return false; }
	virtual bool is_real(){ return false; }
	virtual bool is_bool(){ return false; }
	// QTextStream& echo(QTextStream& os);
	std::string str();
};

struct gx::json::DICT: gx::json, std::map< std::string, gx::json* > {
	bool is_dict(){ return true; }
	void on(gx::json::proc*o){ o->on(this); }
	DICT(){}
};

struct gx::json::LIST: gx::json, std::list<gx::json*> {
	bool is_list(){ return true; }
	void on(gx::json::proc*o){ o->on(this); }
	LIST(){}
};

struct gx::json::NONE: gx::json {
	bool is_null(){ return true; }
	void on(gx::json::proc*o){ o->on(this); }
	NONE(){}
};

struct gx::json::CSTR: gx::json {
	bool is_cstr(){ return true; }
	void on(gx::json::proc*o){ o->on(this); }
	CSTR(const char* src):value(src){}
	std::string value;
};

struct gx::json::SINT: gx::json {
	bool is_sint(){ return true; }
	void on(gx::json::proc*o){ o->on(this); }
	SINT(const int& v):value(v){}
	int value;
};

struct gx::json::UINT: gx::json{
	bool is_uint(){ return true; }
	void on(gx::json::proc*o){ o->on(this); }
	UINT(const unsigned int& v):value(v){}
	unsigned int value;
};

struct gx::json::REAL: gx::json{
	bool is_real(){ return true; }
	void on(gx::json::proc*o){ o->on(this); }
	REAL(const double& v):value(v){}
	double value;
};

struct gx::json::BOOL: gx::json{
	bool is_bool(){ return true; }
	void on(gx::json::proc*o){ o->on(this); }
	BOOL(const bool& v):value(v){}
    bool value;
};

struct gx::json::type_name:gx::json::proc{
    std::string ret;
    virtual void on(gx::json::DICT*){ret="DICT";}
    virtual void on(gx::json::LIST*){ret="LIST";}
    virtual void on(gx::json::NONE*){ret="NONE";}
    virtual void on(gx::json::CSTR*){ret="CSTR";}
    virtual void on(gx::json::SINT*){ret="SINT";}
    virtual void on(gx::json::UINT*){ret="UINT";}
    virtual void on(gx::json::REAL*){ret="REAL";}
    virtual void on(gx::json::BOOL*){ret="BOOL";}
    std::string operator()(gx::json*o){o->on(this); return ret;}
};

struct gx::json::type_id:gx::json::proc{
    int ret;
    virtual void on(gx::json::DICT*){ret=GX_JSON_DICT;}
    virtual void on(gx::json::LIST*){ret=GX_JSON_LIST;}
    virtual void on(gx::json::NONE*){ret=GX_JSON_NONE;}
    virtual void on(gx::json::CSTR*){ret=GX_JSON_CSTR;}
    virtual void on(gx::json::SINT*){ret=GX_JSON_SINT;}
    virtual void on(gx::json::UINT*){ret=GX_JSON_UINT;}
    virtual void on(gx::json::REAL*){ret=GX_JSON_REAL;}
    virtual void on(gx::json::BOOL*){ret=GX_JSON_BOOL;}
    int operator()(gx::json*o){o->on(this);return ret;}
};

struct gx::json::type_guard:gx::json::proc{
    struct unexpected{ std::string str; unexpected(const char* s):str(s){} };
    virtual void on(gx::json::DICT*){throw unexpected("DICT");}
    virtual void on(gx::json::LIST*){throw unexpected("LIST");}
    virtual void on(gx::json::NONE*){throw unexpected("NONE");}
    virtual void on(gx::json::CSTR*){throw unexpected("CSTR");}
    virtual void on(gx::json::SINT*){throw unexpected("SINT");}
    virtual void on(gx::json::UINT*){throw unexpected("UINT");}
    virtual void on(gx::json::REAL*){throw unexpected("DICT");}
    virtual void on(gx::json::BOOL*){throw unexpected("BOOL");}
    virtual ~type_guard(){}
};

//is first step of build gdom instance - parse json tree and build builders

struct gx_json_no_operation_t: gx::json::proc
{
    virtual void on(gx::json::DICT *){}
    virtual void on(gx::json::LIST *){}
    virtual void on(gx::json::NONE *){}
    virtual void on(gx::json::CSTR *){}
    virtual void on(gx::json::SINT *){}
    virtual void on(gx::json::UINT *){}
    virtual void on(gx::json::REAL *){}
    virtual void on(gx::json::BOOL *){}
    virtual ~gx_json_no_operation_t(){}
};

struct gx_json_type_guard_t: gx::json::proc
{
    gx::format_error_t* mp_format_error;

    gx_json_type_guard_t()
        :mp_format_error(NULL)
    {}

    std::string __format_error(const char* wtf){
        if( mp_format_error )
            return mp_format_error->format_error(wtf);
        return std::string(wtf);
    }
    virtual ~gx_json_type_guard_t(){}
    virtual void on(gx::json::DICT *){throw gx::syntax(__format_error("unexpected DICT"));}
    virtual void on(gx::json::LIST *){throw gx::syntax(__format_error("unexpected LIST"));}
    virtual void on(gx::json::NONE *){throw gx::syntax(__format_error("unexpected NONE"));}
    virtual void on(gx::json::CSTR *){throw gx::syntax(__format_error("unexpected CSTR"));}
    virtual void on(gx::json::SINT *){throw gx::syntax(__format_error("unexpected SINT"));}
    virtual void on(gx::json::UINT *){throw gx::syntax(__format_error("unexpected UINT"));}
    virtual void on(gx::json::REAL *){throw gx::syntax(__format_error("unexpected REAL"));}
    virtual void on(gx::json::BOOL *){throw gx::syntax(__format_error("unexpected BOOL"));}
};

#endif // GX_JSON_WRAPPER_H_INCLUDED
