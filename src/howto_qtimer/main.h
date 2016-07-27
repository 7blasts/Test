#ifndef HOWTO_QTIMER_H_INCLUDED
#define HOWTO_QTIMER_H_INCLUDED

#include <QTimer>
#include <QDebug>
#include <QCoreApplication>

class gx_time_implementation : public QObject
{
    Q_OBJECT

private:
    QTimer *timer;
    int updates_count;

public:
    explicit gx_time_implementation(QObject *parent = 0)
        :QObject(parent)
        ,updates_count(0)
    {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(100);
    }

public slots:
    void update()
    {
        qDebug()<<"update <"<<updates_count<<"> called";
        if( ++updates_count > 15 )
        {
            QCoreApplication::instance()->exit(0);
        }
    }
};

#endif  // HOWTO_QTIMER_H_INCLUDED
