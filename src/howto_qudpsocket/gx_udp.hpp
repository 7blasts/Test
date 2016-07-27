#ifndef GX_UDP_HPP_INCLUDED
#define GX_UDP_HPP_INCLUDED

#include <map>
#include <string>

namespace gx
{
  namespace udp
  {
    struct socket;
    struct driver;
    struct setter;

    typedef std::map<int, gx::udp::socket*> sockets_map_t;
    typedef std::map<std::string, gx::udp::setter*> setters_map_t;

    gx::udp::driver* instance();

    struct setter
    {
        virtual const char* get_name()=0;
        virtual void on_incoming_datagram(
                gx::udp::socket*,
                const char* host,
                unsigned short port,
                const char* data,
                int size)=0;
        virtual~setter(){}
    };

    struct socket
    {
    public:
        unsigned short m_port;
        gx::udp::setter* mp_setter;
        gx::udp::driver* mp_driver;
        bool is_wait_data_port(){ return mp_setter ? true : false; }
        virtual~socket(){}
        friend class gx::udp::driver;
    protected:
        socket(const socket&);
        socket()
            : m_port(0)
            , mp_setter(NULL)
            , mp_driver(NULL)
        {
        }
    };

    struct driver
    {
        static sockets_map_t& sockets(){ static sockets_map_t o; return o; }
        static setters_map_t& setters(){ static setters_map_t o; return o; }
        virtual gx::udp::socket* find(int port)=0;
        virtual gx::udp::socket* open(int port)=0;
        virtual gx::udp::socket* open(int port, const char* setter_type_name)=0;
        virtual void done(int port)=0;
        virtual void done(gx::udp::socket*)=0;
    };

  }  // namespace udp
}  // namespace gx 

#endif  // GX_UDP_HPP_INCLUDED
