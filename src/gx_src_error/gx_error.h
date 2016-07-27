<<<<<<< HEAD
#ifndef GX_JSON_ERROR_H_INCLUDED
#define GX_JSON_ERROR_H_INCLUDED

#include <sstream>

namespace gx
{
    struct format_error_t
    {
        // We need maximum info about some syntax error! Like:
        // >> ERROR: ns.gxa_position:f3[200] unexpected LIST
        virtual std::string get_error_path()=0;
        virtual std::string format_item_error(const char* wtf)=0;

        virtual std::string format_error(const char* wtf)
        {
            std::ostringstream oss;
            oss<< get_error_path() << " => " << wtf << "!";
            return oss.str();
        }
        virtual std::string format_code_error(const char* wtf)
        {
            std::ostringstream oss;
            oss<< get_error_path() << "_state_machine => " << wtf << "!";
            return oss.str();
        }
    };  // struct format_error_t
}  // namespace gx

#endif  // GX_JSON_ERROR_H_INCLUDED
=======
#ifndef GX_JSON_ERROR_H_INCLUDED
#define GX_JSON_ERROR_H_INCLUDED

#include <sstream>

namespace gx
{
    struct format_error_t
    {
        // We need maximum info about some syntax error! Like:
        // >> ERROR: ns.gxa_position:f3[200] unexpected LIST
        virtual std::string get_error_path()=0;
        virtual std::string format_item_error(const char* wtf)=0;

        virtual std::string format_error(const char* wtf)
        {
            std::ostringstream oss;
            oss<< get_error_path() << " => " << wtf << "!";
            return oss.str();
        }
        virtual std::string format_code_error(const char* wtf)
        {
            std::ostringstream oss;
            oss<< get_error_path() << "_state_machine => " << wtf << "!";
            return oss.str();
        }
    };  // struct format_error_t
}  // namespace gx

#endif  // GX_JSON_ERROR_H_INCLUDED
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182
