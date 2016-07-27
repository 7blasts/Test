#include <json/json.h>
#include <gx_json.h>

#include <sstream>

std::string gx::json::str(){
	std::ostringstream oss;
	static struct:gx::json::proc{
		std::ostringstream* p;
		void on(gx::json::DICT*o){
			(*p)<< "{";
			if(o->size()){
				gx::json::DICT::iterator curr=o->begin();
				for( int i=o->size(); i>1; ++curr, --i ) {
					(*p)<<"\""<<curr->first.c_str()<<"\":";
					(*this)(*p,curr->second);
					(*p)<<",";
				}
				if( o->end() != curr ) {
					(*p)<<"\""<<curr->first.c_str()<<"\":";
					(*this)(*p,curr->second);
				}
			}
			(*p)<<"}";
		}
		void on(gx::json::LIST*o){
			(*p)<< "[";
			if(o->size()){
				gx::json::LIST::iterator curr=o->begin();
				for( int i=o->size(); i>1; ++curr, --i ) {
					(*this)(*p,*curr);
					(*p)<<",";
				}
				(*this)(*p,*curr);
			}
			(*p)<<"]";
		}
        void on(gx::json::NONE*){(*p)<<"null";}
		void on(gx::json::CSTR*o){(*p)<<"\""<<o->value.c_str()<<"\"";}
		void on(gx::json::SINT*o){(*p)<<o->value;}
		void on(gx::json::UINT*o){(*p)<<o->value;}
		void on(gx::json::REAL*o){(*p)<<o->value;}
		void on(gx::json::BOOL*o){(*p)<<((o->value)?"true":"false");}
		void operator()( std::ostringstream&oss, gx::json* o ){ p=&oss; o->on(this); }
	}print_json;
	print_json( oss, this );
	return oss.str();
}

static gx::json* _from( const Json::Value& value )
{
        switch ( value.type() )
        {
        case Json::nullValue   : return new gx::json::NONE; //NULL
        case Json::intValue    : return new gx::json::SINT ( value.asInt()     );
        case Json::uintValue   : return new gx::json::UINT ( value.asUInt()    );
        case Json::realValue   : return new gx::json::REAL ( value.asDouble()  );
        case Json::stringValue : return new gx::json::CSTR ( value.asCString() );
        case Json::booleanValue: return new gx::json::BOOL ( value.asBool()    );
        case Json::arrayValue  :
                {
                        gx::json::LIST* ls = new gx::json::LIST;
                        int size=value.size();
                        for(int i=0; i<size;++i)
                        {
                                ls->push_back(_from(value[i]));
                        }
                        return ls;
                }
        case Json::objectValue :
                {
                        gx::json::DICT*ns = new gx::json::DICT;
                        Json::Value::Members members( value.getMemberNames() );
                        Json::Value::Members::iterator it = members.begin();
                        Json::Value::Members::iterator no = members.end();
                        for(;it!=no;++it)
                        {
                                const std::string &name = *it;
                                (*ns)[ name ] = _from( value[name] );
                        }
                        return ns;
                }
        }
        return NULL; //impossible to do
}

gx::json* gx::from( const char* s ){
  Json::Value value;
  Json::Reader reader;
  bool parsingSuccessful = reader.parse( s, value );
  if(!parsingSuccessful)
          return NULL;
  return _from(value);
}
