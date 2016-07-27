#include <map>
#include <sstream>

#include <gx_json.h>

struct gx_json_free:gx::json::proc
{ 
	void on(gx::json::DICT*o)
	{
		gx_json_free kill;
		gx::json::DICT::iterator iter=o->begin();
		gx::json::DICT::iterator stop=o->end();
		for(;iter!=stop;++iter)
		{
			kill ( iter->second );
		}
		o->clear();
    }
	void on(gx::json::LIST*o)
	{
		gx_json_free kill;
		gx::json::LIST::iterator iter=o->begin();
		gx::json::LIST::iterator stop=o->end();
		for(;iter!=stop;++iter)
		{
			kill ( (*iter) );
		}
		o->clear();
    }
    void on(gx::json::NONE*){}
    void on(gx::json::CSTR*){}
    void on(gx::json::SINT*){}
    void on(gx::json::UINT*){}
    void on(gx::json::REAL*){}
    void on(gx::json::BOOL*){}
	void operator()(gx::json*o)
	{
		o->on(this);
		delete o;
	}
};

void gx::json::free(gx::json *o){
    static gx_json_free kill;
    kill(o);
}

