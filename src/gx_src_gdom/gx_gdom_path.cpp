<<<<<<< HEAD
#include <sstream>

#include "gx_gdom.h"

namespace gx{
	    inline std::ostringstream& reset(std::ostringstream&oss){
        oss.clear(); oss.str(""); return oss;
    }
}

void gx::path::parse(const std::string &src){
    std::size_t pos, head = 0;
    while(1){
        pos = src.find('.', head);
        if ( pos == std::string::npos )
            break;
        //std::cout<<src.substr(head, pos-head)<<" <" <<head<<", "<<pos<<">"<<std::endl;
        data.push_back(src.substr(head, pos-head));
        head = pos+1;
    }
    data.push_back(src.substr(head));
    //std::cout<<src.substr(head)<<" <" <<head<<", "<<pos<<">"<<std::endl;
}

void gx::path::set_path(const std::string& src){
	data.clear();
	parse(src);
}
gx::path::path(const std::string& src) {  parse(src); }
gx::path::path(const char* src) { parse(src); }

std::string gx::path::info(){
    std::ostringstream oss;
    oss<<"gx.path at<"<<this<<">:"<<std::endl;
		data_t::iterator iter = data.begin(), none = data.end();
    for(;iter!=none;++iter){
        oss<<"  -\"" <<*iter<<"\""<<std::endl;
    }
    return oss.str();
}

std::string gx::path::repr(){
    std::ostringstream oss;
		data_t::iterator iter = data.begin(), none = data.end();
    std::size_t i=0;
    for(;iter!=none;++iter,++i){
        oss<<"."<<*iter;
    }
    std::string str = oss.str();
    if( i ){
       return str.substr(1);
    }
    return str;
}

//TODO: remove debug stdout and iostream, reuse ostringstream as last error
#include <iostream>

std::string gx::path::subpath(const std::size_t& last){
    std::ostringstream oss;
		data_t::const_iterator iter = data.begin(), none = data.end();
    std::size_t i=0;
    for(; iter!=none; ++iter, ++i ){
        if( i>=last )
            break;
        oss<<"."<<*iter;
    }
    std::string str = oss.str();
    if( i ){
        return str.substr(1);
    }
    return str;
}

// use this path starting dict used as argument:
gx::xx* gx::path::find(gx::ns* base)
{
		static gx::is_it_type<gx::ns> is_ns_ptr; // functor(gx::xx*o), return true 'o' some 'ns*'

		gx::ns* curr = base;

		data_t::iterator iter = data.begin(), none = data.end();
    for(std::size_t i=1; iter != none; ++iter, ++i)
		{
        const std::string& key = (*iter);

        if( 0 == key.size() ){
            curr = curr->nest;
            if( curr == NULL){
                // ERROR: object at path %path% has no parent
                gx::reset(err)<<"ERROR: object at path '"<<subpath(i)<<"' has no parent";
                return NULL;
            }
        }
        else
				{
					gx::ns::data_t::iterator subj = curr->data.find(key);
            if( subj == curr->data.end() ){
                // ERROR: ns at path %path% don't has key %key%
                gx::reset(err)<<"ERROR: ns at path '"<<subpath(i-1)<<"' don't has key '"<<key<<"'";
                return NULL;
            }
            gx::xx* o = subj->second;
            if( i == data.size() )
						{
                gx::reset(err);
                return o; // last key success, but if object not found returned value is NULL;
            }
						if( is_ns_ptr(o) )
						{
							curr = (gx::ns*)o;
						}
						else
						{
							gx::reset(err)<<"ERROR: object at path '"<<subpath(i)<<"' are not some ns";
							return NULL;
						}
        }
    }
    gx::reset(err);
    return curr;
}
=======
#include <sstream>

#include "gx_gdom.h"

namespace gx{
	    inline std::ostringstream& reset(std::ostringstream&oss){
        oss.clear(); oss.str(""); return oss;
    }
}

void gx::path::parse(const std::string &src){
    std::size_t pos, head = 0;
    while(1){
        pos = src.find('.', head);
        if ( pos == std::string::npos )
            break;
        //std::cout<<src.substr(head, pos-head)<<" <" <<head<<", "<<pos<<">"<<std::endl;
        data.push_back(src.substr(head, pos-head));
        head = pos+1;
    }
    data.push_back(src.substr(head));
    //std::cout<<src.substr(head)<<" <" <<head<<", "<<pos<<">"<<std::endl;
}

void gx::path::set_path(const std::string& src){
	data.clear();
	parse(src);
}
gx::path::path(const std::string& src) {  parse(src); }
gx::path::path(const char* src) { parse(src); }

std::string gx::path::info(){
    std::ostringstream oss;
    oss<<"gx.path at<"<<this<<">:"<<std::endl;
		data_t::iterator iter = data.begin(), none = data.end();
    for(;iter!=none;++iter){
        oss<<"  -\"" <<*iter<<"\""<<std::endl;
    }
    return oss.str();
}

std::string gx::path::repr(){
    std::ostringstream oss;
		data_t::iterator iter = data.begin(), none = data.end();
    std::size_t i=0;
    for(;iter!=none;++iter,++i){
        oss<<"."<<*iter;
    }
    std::string str = oss.str();
    if( i ){
       return str.substr(1);
    }
    return str;
}

//TODO: remove debug stdout and iostream, reuse ostringstream as last error
#include <iostream>

std::string gx::path::subpath(const std::size_t& last){
    std::ostringstream oss;
		data_t::const_iterator iter = data.begin(), none = data.end();
    std::size_t i=0;
    for(; iter!=none; ++iter, ++i ){
        if( i>=last )
            break;
        oss<<"."<<*iter;
    }
    std::string str = oss.str();
    if( i ){
        return str.substr(1);
    }
    return str;
}

// use this path starting dict used as argument:
gx::xx* gx::path::find(gx::ns* base)
{
		static gx::is_it_type<gx::ns> is_ns_ptr; // functor(gx::xx*o), return true 'o' some 'ns*'

		gx::ns* curr = base;

		data_t::iterator iter = data.begin(), none = data.end();
    for(std::size_t i=1; iter != none; ++iter, ++i)
		{
        const std::string& key = (*iter);

        if( 0 == key.size() ){
            curr = curr->nest;
            if( curr == NULL){
                // ERROR: object at path %path% has no parent
                gx::reset(err)<<"ERROR: object at path '"<<subpath(i)<<"' has no parent";
                return NULL;
            }
        }
        else
				{
					gx::ns::data_t::iterator subj = curr->data.find(key);
            if( subj == curr->data.end() ){
                // ERROR: ns at path %path% don't has key %key%
                gx::reset(err)<<"ERROR: ns at path '"<<subpath(i-1)<<"' don't has key '"<<key<<"'";
                return NULL;
            }
            gx::xx* o = subj->second;
            if( i == data.size() )
						{
                gx::reset(err);
                return o; // last key success, but if object not found returned value is NULL;
            }
						if( is_ns_ptr(o) )
						{
							curr = (gx::ns*)o;
						}
						else
						{
							gx::reset(err)<<"ERROR: object at path '"<<subpath(i)<<"' are not some ns";
							return NULL;
						}
        }
    }
    gx::reset(err);
    return curr;
}
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182
