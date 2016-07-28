#include <QCoreApplication>
#include <QThread>

#include <QUdpSocket>
#include <QTextStream>

/* Здравствуйте. Пишу udp сервер и с толкнулся с такой проблемой: сервер запущен в отдельном потоке и принимает  датаграммы
 * каждые 20 mS. Приоритет потока поднят до приоритета реального времени (QThread::TimeCriticalPriority). Чтение поступивших
 * данных осуществляю по сигналу readyRead(). Пакеты  приходят, сервер их разгребает. Все хорошо до того момента, пока я не
 * запускаю браузер на этой же машине. В момент старта браузера происходит задержка пакетов (delay spike), а затем они все
 * сваливаются на сервер. Теперь о проблеме: задержка в момент запуска браузера (измеренная c  помощью WireShark) составляет
 * около 200 mS.  Эта же задержка, измеренная с помощью QElapsedTimer в слоте сервера составляет около 800-900  mS.
 * Грешу на network модуль от Qt (а точнее на поток), в котором происходит отслеживание состояния сокетов в ОС.
 * В process explorer вижу свой поток, который запущен с TimeCriticalPriority приоритетом и поток обработки событий сокетов,
 * у которого нормальный приоритет. И он, похоже, тормозит все дело. Есть ли возможность поднять его приоритет?
 *
 *


*/

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    QByteArray datagram;
    QHostAddress sender;
    quint16 senderPort;
    QTextStream qout(stdout);
    QUdpSocket *udpSocket = new QUdpSocket(0);
    udpSocket->bind(45455, QUdpSocket::ShareAddress);
    //udpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 1048576);
    //udpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    while(1)
    {
        while (udpSocket->waitForReadyRead(1))
        {
            int x=0;
            while(udpSocket->hasPendingDatagrams())
            {
                if(x++>4)
                {
                    x=0;
                    qout<<endl;
                    break;
                }
                datagram.resize(udpSocket->pendingDatagramSize());
                udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
                qout << "<" << sender.toString()<< ":" << senderPort << ">"
                     << datagram.size() << "b "
                     << datagram.data() << endl;
            }
            break;
        }
        // qout << "<processEvents>" << endl;
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        // p_main_thread->msleep(3);
    }
}
