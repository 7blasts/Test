#include <map>
#include <set>
#include <string>
#include <cstdlib>
#include <iostream>

#include <QCoreApplication>
#include <QThread>

#include <QUdpSocket>
#include <QTextStream>

#include <gx_json.h>

/* Модель данных содержит только значения связанные в DG (граф зависимостей). В "чистом"
 * виде "модели вычислений с покраской", установка любого значения окрашивает флагом
 * "изменён" все значения вычисляемые из данного. Но реализационно, вычислитель одного
 * или нескольких значений DG, происходящий от базового абстрактного класса FN, содержит
 * все рёбра графа зависимостей. По идее, каждое значение должно содержать ссылку на свой
 * "вычислитель", но в данной реализации это заменено на путь к узлу в дереве значений,
 * завершаемый именем вычислителя. Инстанциирование узла приведёт к инстанциированию
 * вычислителя, затем к инстанциированию всех его входящих "get" и исходящих "set" рёбер
 * графа зависимостей. Для предотвращения бессмысленной и беспощадной загрузки всего графа
 * зависимостей, вся модель данных делится горизонтально на кластеры, а вертикально на
 * динамически присоединяемые аспекты представлений. Причем каждое представление может
 * являться моделью данных и в свою очередь иметь представление(вид)
 * ----
 * С моделью данных связан FN - получающий два вида сигналов:
 * -1- "ИЗМЕНИЛИСЬ ЗНАЧЕНИЯ"
 * -2- "ИЗМЕНИЛАСЬ ЛИНКОВКА":
 *    Замена указателя в пространстве имён - изменяет топологию DG (графа зависимостей).
 *    Сообщает о том что сам маршрут вычислений в DG, непосредственно в данном узле,
 *    уже изменён внешним алгоритмом. Для узлов зависящих от данного далее распространяется
 *    только сигнал "ИЗМЕНИЛИСЬ ЗНАЧЕНИЯ".
 * ----
 * Изменения линковки происходят при внесении\удалении объектов в gdom (DAG) при условии,
 * что существует code - объект (FN) использующий изменяемую область DG. Следовательно
 * объекты code потерпевшие крах на этапе линковки, могут отложить свою перелинковку опираясь
 * на время (перепослать событе себе по таймеру), или по факту изменения в ветке DAG. Второе
 * потребует обязательного создания <code-объект> получающего сигнал об изменении одной или
 * нескольких веток DAG. Такой <code-объект> будем называть ловушкой <trap-объект>.
 * ----
 * Запись данных в gdom (DAG) может осуществляться только миниатюрными порциями, требование
 * вытекащее из условий системы реального времени. Если алгоритм требует создания большого
 * монолитного массива данных, создавать его нужно в отдельном пространстве имён, и только
 * по готовности к работе заменять ссылку на ветку gdom (DAG):
 * NEW(temp, "u32:700000")
 * PAR
 *   SET(temp, {"00000000:":[2,14,251]})
 *   SET(temp, {"00000100-FF":["u32",700]})
 *   SET(temp, {"00000F00:":["",2,14,251]"randu32"})
 * SEQ
 *   SET(path, temp)
 */

namespace gx{
    namespace udp {
        struct socket_t;                          // Abstract socket
        struct sender_host_data_t;                // Remote host data state
        struct sender_port_data_t;                // Remote port data state
        struct session_t;                         // Host-port Single session
        typedef std::set<session_t*> sessions_t;  // Singletoned sessions set
        struct reactor_t;                         // Reactor
        typedef reactor_t* (*reactor_factory_t)(const char* json);  // Reactor's factory
        typedef std::map<std::string, reactor_factory_t> reactors_t;  // Reactors map
        sessions_t& sessions();  // All active sessions (sigleton)
        reactors_t& reactors();  // All reactors                      reactors();
        std::map<std::string, reactor_t>& protocols();
        struct reactor_t                   // Factory
        {     // extract sender host, extract sender port, find session_state, call reactor
            virtual void read(gx::udp::socket_t* sock,     // socket implementation instance
                              const char* data,            // buffer
                              const unsigned long& size,   // buffer size
                              const std::string& r_host,   // str sender host name
                              const unsigned short& r_port // str sender host name
                              )
            {
                qDebug() << sock << data << size << r_host.c_str() << r_port;
            }
        };

        struct sender_port_data_t{  // current interface for sender <host:port>
            typedef std::map<std::string, reactor_t*> reactors_t;
            reactors_t reactors;
            virtual void read(gx::udp::socket_t* sock,     // socket implementation instance
                              const char* data,
                              const unsigned long& size,
                              const std::string& r_host,
                              const unsigned short& r_port
                              //const char* data,            // buffer
                              //const unsigned long& size,   // buffer size
                              //const std::string& r_host,   // str sender host name
                              //const unsigned short& r_port // str sender host name
                              )
            {
                qDebug() << sock << data << size << r_host.c_str() << r_port;
            }
        };

        struct sender_host_data_t{
            std::map<unsigned short, gx::udp::sender_port_data_t> sender_ports;
            sender_port_data_t* find_port(const unsigned short& p){
                std::map<unsigned short, gx::udp::sender_port_data_t>::iterator
                        subj = sender_ports.find(p);
                if( sender_ports.end() == subj ) {
                    return NULL;
                }
                return &subj->second;
            }
        };

        struct socket_t {  // base class for platform depended implementation
            std::map<std::string, reactor_t*> sessions_map;  // multi-host sessions
            std::map<std::string, sender_host_data_t> hosts_map;
            unsigned short self_port=0;
            std::string self_host;
            socket_t(const unsigned short& s_port)
                :self_port(s_port)
                , self_host("127.0.0.1")
            {
            }
            virtual ~socket_t(){}
            reactor_t* find_reactor(const std::string& name){
                std::map<std::string, reactor_t*>::iterator subj = sessions_map.find(name);
                return (sessions_map.end()==subj) ? NULL : subj->second;
            }
            void read(
                    const char* data,
                    const unsigned long& size,
                    const std::string& r_host,
                    const unsigned short& r_port
                    )
            {
                reactor_t* reactor;
                std::map<std::string, sender_host_data_t>::iterator i_host = hosts_map.find(r_host);
                if(hosts_map.end() == i_host){
                    std::cout<< "0:on-unknown-host(" << r_host << ")\n";
                    reactor = find_reactor("0:on-unknown-host");
                    if(!reactor)
                    {
                        std::cout<< "ERROR: Reactor undefined '0:on-unknown-host'";
                        return;
                    }
                    reactor->read(this, data, size, r_host, r_port);
                }
                sender_port_data_t* p_port_data = i_host->second.find_port(r_port);
                if(!p_port_data){
                    reactor = find_reactor("1:on-unknown-process");
                    if(!reactor)
                    {
                        std::cout<< "ERROR: Reactor undefined '1:on-unknown-process'";
                        return;
                    }
                }
                p_port_data->read(this, data, size, r_host, r_port);
            }
            virtual void send(const char* data, const unsigned long& size)=0;
        };  // gx::udp::socket

        // To manage instance of the platform depended socket implementatin use list of
        // "gx::udp::socket_declaration"s. System must scan this list in main loop and
        // visit each "socket_declaration", to create, destroy or call main loop for each
        // socket implementatin in this list.
        struct session_t  // Session is more stable variable, then socket connection.
        {                 // So can wait for socket creation/recreation and destroy.
            void on_socket_implementation_new(){}
            void on_socket_implementation_del(){}

            std::map<std::string, reactor_t*> sessions_map;  // multi-host sessions
            std::map<std::string, sender_host_data_t> hosts_map;

            bool done;  // True mean cmd for main loop => clean up and delete implementation
                        // if (implementation == NULL) => operation is done, declaration ready for remove
            unsigned long refs;                 // zero refs mean => after remove from sessions => delete instance
            gx::udp::socket_t* implementation;  // pointer to instatnce of the system depended implementation
            unsigned int port;                  // port number, 0 mean => bind port not needed
            std::string initial_protocol_name;  // a name of the initial protocol's factory

            reactor_t* find_reactor(const std::string& name){
                std::map<std::string, reactor_t*>::iterator subj = sessions_map.find(name);
                return (sessions_map.end()==subj) ? NULL : subj->second;
            }
            void read(
                    const char* data,
                    const unsigned long& size,
                    const std::string& r_host,
                    const unsigned short& r_port
                    )
            {
                reactor_t* reactor;
                std::map<std::string, sender_host_data_t>::iterator i_host = hosts_map.find(r_host);
                if(hosts_map.end() == i_host){
                    std::cout<< "0:on-unknown-host(" << r_host << ")\n";
                    reactor = find_reactor("0:on-unknown-host");
                    if(!reactor)
                    {
                        // For example: Before use STUN query from this host, '0:on-unknown-host' must be
                        // extended with STUN echo parser. After success STUN - IP detection , is must
                        //
                        //>>> import socket
                        //>>> [ socket.gethostbyname("stun%s.l.google.com"%i) for i in ["","1","2","3","4"] ]
                        //['64.233.161.127', '74.125.200.127', '74.125.204.127', '74.125.204.127', '64.233.165.127']
                        //'64.233.161.127':,, '74.125.200.127', '74.125.204.127', '74.125.204.127', '64.233.165.127'
                        std::cout<< "ERROR: Reactor undefined '0:on-unknown-host'";
                        return;
                    }
                    reactor->read(implementation, data, size, r_host, r_port);
                }
                sender_port_data_t* p_port_data = i_host->second.find_port(r_port);
                if(!p_port_data){
                    reactor = find_reactor("1:on-unknown-process");
                    if(!reactor)
                    {
                        std::cout<< "ERROR: Reactor undefined '1:on-unknown-process'";
                        return;
                    }
                }
                p_port_data->read(implementation, data, size, r_host, r_port);
            }
            explicit session_t(const std::string& protocol="", const unsigned short& port = 0)
                :done(false)
                ,implementation(NULL)
                ,port(port)
                ,initial_protocol_name( (protocol=="") ? "query_interface":protocol )
            {
                gx::udp::sessions().insert(this);
            }
            ~session_t()
            {
                gx::udp::sessions().erase(this);
            }
        };
        std::set<session_t*>& sessions()  // return ref to std::set of the all active UDP sessions
        {
            static std::set<session_t*> active_udp_sessions; return active_udp_sessions;
        }
    }  // namespace udp
}  // namespace gx


struct QtImplementationGxUdpSocket: gx::udp::socket_t
{
    QByteArray          datagram;  // temporary recived bytes
    QHostAddress        r_host;    // temporary sender's host
    quint16             r_port;    // temporary sender's port
    QUdpSocket          socket;    // implementation
    gx::udp::session_t* self;      // contain self.port, self.done, self reactors state etc..

    QtImplementationGxUdpSocket(gx::udp::session_t* self)
        :gx::udp::socket_t(self->port)
        ,self(self)
    {
    }
    gx::udp::socket_t* init() // try to apply all socket-declaration-side attributes, return NULL on fail
    {
        if (self->port) {
            socket.bind(self_port, QUdpSocket::ShareAddress);
            socket.setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 1048576);
            socket.setSocketOption(QAbstractSocket::LowDelayOption, 1);
        }
        return this;
    }
    void each_socket_proc(unsigned long max_datagrams = 4)
    {
        if( socket.waitForReadyRead(1) )
        {
            unsigned long x=0;
            while(socket.hasPendingDatagrams())
            {
                if(x++>max_datagrams)
                {
                    x=0;
                    break;
                }
                datagram.resize(socket.pendingDatagramSize());
                socket.readDatagram(datagram.data(), datagram.size(), &r_host, &r_port);
                self->read(datagram.data(), datagram.size(), r_host.toString().toStdString(), r_port);
            }
        }
    }
    static void main_loop_proc()
    {
        std::set<gx::udp::session_t*>::iterator
                curr = gx::udp::sessions().begin(),
                none = gx::udp::sessions().end();
        for(; curr != none; ++curr)
        {
            gx::udp::session_t* p_session = *curr;
            if (p_session->done)
            {
                if (p_session->implementation)
                {
                    delete p_session->implementation;
                    p_session->implementation = NULL;
                    p_session->on_socket_implementation_del();
                }
                continue;
            }
            if (NULL == p_session->implementation)
            {
                QtImplementationGxUdpSocket* impl = new QtImplementationGxUdpSocket(p_session);
                p_session->implementation = impl->init();
                if(NULL == p_session->implementation)
                {
                    delete impl;
                    continue;
                }
                p_session->on_socket_implementation_new();
            }
            // Done is False AND implementation is non-zero pointer to
            // QtImplementationGxUdpSocket instanse, so type cast w/o any check:
            QtImplementationGxUdpSocket* p_qt_implementation = reinterpret_cast
                    <QtImplementationGxUdpSocket*>(p_session->implementation);
            p_qt_implementation->each_socket_proc();
        }
    }
    void send(const char* data, const unsigned long& size){
         Q_UNUSED(data); Q_UNUSED(size);
    }
};


int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);  // Application instance

    gx::udp::session_t session("STUN", 45456);  //port opened all time of the programm is working
    QtImplementationGxUdpSocket::main_loop_proc();


    QByteArray datagram;
    std::map<unsigned long, QByteArray> file_dump;
    QHostAddress sender;
    quint16 senderPort;
    QTextStream qout(stdout);
    QUdpSocket *udpSocket = new QUdpSocket(0);
    udpSocket->bind(QHostAddress::LocalHost, 45455, QUdpSocket::DefaultForPlatform); // QUdpSocket::ShareAddress);
    // udpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 1048576);
    // udpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    while(1)
    {
        while(udpSocket->waitForReadyRead(1))
        {
            int x=0;
            while(udpSocket->hasPendingDatagrams())
            {
                if(x++>4)
                {
                    x=0;
                    break;
                }
                datagram.resize(udpSocket->pendingDatagramSize());
                udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

                //
                //on_incoming_datagram(datagram.data(), datagram.size(), sender.toString().toStdString(), senderPort );

                /*   UDP sender send JSON or binary data. UDP reciever do not know how much datagrams
                 * are lost in the network. And how much received from pevious, already closed session.
                 */
                switch (datagram.data()[0])
                {
                    // Data-chunk starts from hex digit, possible is a  binary data-chunk's prefix.

                    case '0': case '1': case '2': case '3':
                    case '4': case '5': case '6': case '7':
                    case '8': case '9': case 'A': case 'B':
                    case 'C': case 'D': case 'E': case 'F':
                    {
                        // In this case prefix contain at least 1 group from 8 hex digits
                        static char* pEnd;
                        if(8 > datagram.size()){
                            continue;  // datagram is too short to be some binary data-chunk
                        }
                        std::string chunk_head(datagram.data(),8);
                        unsigned long chunk_id = strtoul(chunk_head.c_str(), &pEnd, 16);
                        if(8 != pEnd - chunk_head.c_str()){
                            continue;  // head must contain only hexgigits symbols
                        }
                        //std::map<std::string, gx:: unsigned long
                        // If successfuly opened session for this host not exists
                        std::map<unsigned long, QByteArray>::iterator chunk_iterator = file_dump.find(chunk_id);
                        if(file_dump.end() != chunk_iterator){
                            continue;  // skip dublicates
                        }
                        // Store chunk to 'file_dump':
                        // At this step we don't know type or usage of this file,
                        //   but later we can use something like:
                        // bool QImage::loadFromData(const QByteArray &data, const char *format = Q_NULLPTR);
                        //   to detect is usable as QImage or not, is compilable as shader, or JSON e.t.c ...
                        file_dump[chunk_id] = QByteArray::fromRawData(datagram.data()+8, datagram.size()-8);
                        qout << "from " << sender.toString()
                             << ":"     << senderPort
                             << ", id=" << chunk_id
                             << ", sz=" << datagram.size()
                             << ", sd=" << file_dump[chunk_id].size() <<"\n";
                        qout << file_dump[chunk_id].data() << "\n\n";
                    } break;  // inline case is hex digit

                    default:  // in all other cases we accept only JSON-dict or JSON-list based messages
                    {
                        gx::json* parsed = gx::from(datagram.data());
                        if(NULL == parsed)
                        {
                            qout << "IS NOT JSON\n";
                            break;
                        }
                        else
                        {  //  JSON?
                            qout << "JSON based message at <" << parsed << ">\n";
                            if (parsed->is_dict())  // DICT?
                            {
                                qout << "JSON dict msg at <" << parsed << ">\n";
                                gx::json::DICT* p_dict = (gx::json::DICT*) parsed;
                                gx::json::DICT::iterator subj, none = p_dict->end();
                                // Detect query interface, use field "mtype" with type "cstr"
                                subj = p_dict->find("mtype");  // is query interface message?
                                if((subj != none)&&(subj->second->is_cstr()))
                                {
                                    std::string mtype(((gx::json::CSTR*)subj->second)->value);
                                    qout << "     'mtype':'"<< mtype.c_str() <<"'\n";
                                    if ("FtpReceiver" == mtype)
                                    {
                                        /*query+cstr+?*/subj = p_dict->find("query");
                                        if((subj != none)&&(subj->second->is_cstr())){
                                            std::string query(((gx::json::CSTR*)subj->second)->value);
                                            qout << "     'query':'"<< query.c_str() <<"'\n";
                                        }
                                        /*codec+cstr+?*/subj = p_dict->find("codec");
                                        if((subj != none)&&(subj->second->is_cstr())){
                                            std::string codec(((gx::json::CSTR*)subj->second)->value);
                                            qout << "     'codec':'"<< codec.c_str() <<"'\n";
                                        }
                                        // ... FtpReceiver ...
                                        // If interface already opened for sender<host:port> send dublicated accept
                                        // If interface already closed for sender<host:port> send dublicated reject
                                        // If interface queried from sender<host:port> for first time, make decision
                                        static QByteArray FtpReceiver_echo("{\"start_pos\":0,\"step\":3}");
                                        udpSocket->writeDatagram(FtpReceiver_echo, sender, senderPort);
                                        qout.flush();
                                        continue;
                                    }
                                    if("FtpReceiverParts" == mtype){
                                        // ... FtpReceiver->parts ...
                                        static QByteArray echo_dict("{\"parts\":[4,5,6],\"freestep\":3}");
                                        udpSocket->writeDatagram(echo_dict, sender, senderPort);
                                    }
                                }
                            }
                            else if(parsed->is_list())
                            {
                                qout << "JSON list msg at <" << parsed << ">\n";
                                static QByteArray echo_list("{\"parts_responce\":true}");
                                udpSocket->writeDatagram(echo_list, sender, senderPort);
                            }
                            else
                            {
                                qout << "ERROR:Unsupported JSON message's type at <" << parsed << ">\n";
                            }
                        }
                    }  // end switch-default
                }  // end switch
                qout.flush();
            }
            break;
        }
        // qout << "<processEvents>" << endl;
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        // p_main_thread->msleep(3);
    }
}
