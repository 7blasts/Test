#ifndef GX_UDP_QT_DRIVER_H_INCLUDED
#define GX_UDP_QT_DRIVER_H_INCLUDED

#include <assert.h>

#include "gx_udp.hpp"

#include <stdio.h>
#include <QDebug>
#include <QByteArray>
#include <QCoreApplication>
#include <QThread>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QUdpSocket>

class gx_udp_socket_qt_implementation: public QObject, public gx::udp::socket
{
    Q_OBJECT

public:

    QUdpSocket *mp_qt_udp_socket;

    explicit gx_udp_socket_qt_implementation(QObject* parent = NULL): QObject(parent)
    {
        mp_qt_udp_socket = new QUdpSocket(this);
    }
    
    void connect_ready_read()
    {

        //mp_qt_udp_socket -> bind(QHostAddress::LocalHost, m_port);
        mp_qt_udp_socket -> bind( m_port, QUdpSocket::ShareAddress);
        mp_qt_udp_socket -> setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 1048576);
        mp_qt_udp_socket -> setSocketOption(QAbstractSocket::LowDelayOption, 1);
        connect(mp_qt_udp_socket, SIGNAL(readyRead()), this, SLOT(read_all_pending_datagrams()), Qt::DirectConnection);
    }

public slots:

    void read_all_pending_datagrams()
    {
        QByteArray data;
        QHostAddress sender;
        quint16 senderPort;
        //char buff[4096];
        printf("\n\nRead <%p>", this);
        int x=0;
        while( mp_qt_udp_socket->hasPendingDatagrams() )
        {
            if( 4 < x++ )
            {
                x=0;
                QThread* t = QCoreApplication::instance()->thread();
                t->msleep(10);
                break;
                //QCoreApplication::processEvents(QEventLoop::AllEvents);
                //printf("\n\nRead 2<%p>", this);
            }
            data.resize(mp_qt_udp_socket->pendingDatagramSize());
            mp_qt_udp_socket->readDatagram(data.data(), data.size(), &sender, &senderPort);

            //mp_qt_udp_socket->readDatagram(buff, 4096, &sender, &senderPort);

            if( mp_setter )
            {
                const char* host = sender.toString().toStdString().c_str();
                mp_setter->on_incoming_datagram(this, host, senderPort, data.constData(), data.size());
                //mp_setter->on_incoming_datagram(this, host, senderPort, buff, 4096);
            }
        }
    }
};


class gx_udp_driver_qt_implementation: public QObject, public gx::udp::driver
{
    Q_OBJECT
public:
    gx::udp::socket* find(int port)
    {
        gx::udp::sockets_map_t::iterator exists = sockets().find(port);
        return ( exists == sockets().end() ) ? NULL : exists->second;
    }

    gx::udp::socket* open(int port)
    {
        gx::udp::sockets_map_t::iterator exists = sockets().find(port);
        if( sockets().end() != exists ){
            return exists->second;
        }
        gx_udp_socket_qt_implementation* p_gx_udp_socket = new gx_udp_socket_qt_implementation;
        if( NULL == p_gx_udp_socket ){
            return NULL;
        }
        p_gx_udp_socket -> m_port = port;
        p_gx_udp_socket -> mp_driver = this;
        sockets()[port] = p_gx_udp_socket;
        return p_gx_udp_socket;
    }

    gx::udp::socket* open(int port, const char* setter_type_name)
    {
        gx_udp_socket_qt_implementation *p_gx_udp_socket=NULL;
        gx::udp::setters_map_t::iterator exists_setter_iterator = setters().find(setter_type_name);
        if( setters().end() == exists_setter_iterator ){
            return NULL;
        }
        gx::udp::sockets_map_t::iterator exists_socket_iterator = sockets().find(port);
        if( sockets().end() != exists_socket_iterator )
        {
            p_gx_udp_socket = (gx_udp_socket_qt_implementation*)exists_socket_iterator->second;
            if( p_gx_udp_socket -> is_wait_data_port() )
            {
                p_gx_udp_socket -> mp_setter = exists_setter_iterator->second;
            }
            else
            {
                p_gx_udp_socket -> mp_setter = exists_setter_iterator->second;
                p_gx_udp_socket -> connect_ready_read();
            }
            return p_gx_udp_socket;
        }
        p_gx_udp_socket = new gx_udp_socket_qt_implementation;
        if( NULL == p_gx_udp_socket ){
            return NULL;
        }
        p_gx_udp_socket -> m_port = port;
        p_gx_udp_socket -> mp_driver = this;
        p_gx_udp_socket -> mp_setter = exists_setter_iterator->second;
        p_gx_udp_socket -> connect_ready_read();

        sockets()[port] = p_gx_udp_socket;

        return p_gx_udp_socket;
    }

    void done(int){
        assert(0);
    }

    void done(gx::udp::socket*){
        assert(0);
    }

    gx_udp_driver_qt_implementation(QObject *parent=NULL)
        :QObject(parent)
    {

    }
    ~gx_udp_driver_qt_implementation()
    {

    }
};

#endif  // GX_UDP_QT_DRIVER_H_INCLUDED
