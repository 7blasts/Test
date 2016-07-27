#include <QCoreApplication>

#include "gx_udp_qt_driver.h"
#include <stdio.h>
#include <QThread>


gx::udp::driver* gx::udp::instance()
{
    static gx_udp_driver_qt_implementation i; return &i;
}

struct setter: gx::udp::setter {
    const char* get_name(){ return "print_to_qdebug"; }
    void on_incoming_datagram(
            gx::udp::socket*p_socket,
            const char* host,
            unsigned short port,
            const char* data,
            int size)
    {
        //qDebug()<< "on_incoming_datagram";
        //qDebug()<< "    gx::udp::socket*=" << p_socket;
        //qDebug()<< "    const char*     =" << host;
        //qDebug()<< "    unsigned short  =" << port;
        //qDebug()<< "    const char*     =" << data;
        printf("\n%s", data);
        //qDebug()<< "    int             =" << size;
    }
};

/* Здравствуйте. Пишу udp сервер и с толкнулся с такой проблемой: сервер запущен в отдельном потоке и принимает  датаграммы
 * каждые 20 mS. Приоритет потока поднят до приоритета реального времени (QThread::TimeCriticalPriority). Чтение поступивших
 * данных осуществляю по сигналу readyRead(). Пакеты  приходят, сервер их разгребает. Все хорошо до того момента, пока я не
 * запускаю браузер на этой же машине. В момент старта браузера происходит задержка пакетов (delay spike), а затем они все
 * сваливаются на сервер. Теперь о проблеме: задержка в момент запуска браузера (измеренная c  помощью WireShark) составляет
 * около 200 mS.  Эта же задержка, измеренная с помощью QElapsedTimer в слоте сервера составляет около 800-900  mS.
 * Грешу на network модуль от Qt (а точнее на поток), в котором происходит отслеживание состояния сокетов в ОС.
 * В process explorer вижу свой поток, который запущен с TimeCriticalPriority приоритетом и поток обработки событий сокетов,
 * у которого нормальный приоритет. И он, похоже, тормозит все дело. Есть ли возможность поднять его приоритет?
  */

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);



    static setter print_to_qdebug;
    gx::udp::driver::setters()["print_to_qdebug"]=&print_to_qdebug;

    gx::udp::socket* wait_data = gx::udp::instance()->open(45455,"print_to_qdebug");
    gx::udp::socket* wait_data1 = gx::udp::instance()->open(45456,"print_to_qdebug");
    //gx::udp::socket* send_data = gx::udp::instance()->open(5006);

    QThread* p_main_thread = QCoreApplication::instance()->thread();
    QThread* p_wrap_thread = ((gx_udp_socket_qt_implementation*)wait_data)->thread();
    QThread* p_sock_thread = ((gx_udp_socket_qt_implementation*)wait_data)->mp_qt_udp_socket->thread();

    qDebug()<< "initial priority ="<< p_main_thread->priority();
    p_main_thread->setPriority(QThread::TimeCriticalPriority);
    qDebug()<< "current priority ="<< p_main_thread->priority();

    qDebug()<< "open(45455,\"gdom_json_server\") =>" << wait_data;
    qDebug()<< "open(45456,\"gdom_json_server\") =>" << wait_data1;
    //qDebug()<< "open(5006) \"gdom_json_writer\") =>" << send_data;

    return app.exec();
}
