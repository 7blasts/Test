#include <QProcess>
#include <QDebug>
#include <QTimer>

int main(int, char **)
{
    int vtpp_return_code = 0;
    QProcess vtpp;
    QString exec = "tracert";
    QStringList params;

    qDebug() << "VTPP Before Started";

    params << "-d" << "www.ya.ru";
    vtpp_return_code = vtpp.execute(exec, params);
    vtpp.close();
    qDebug() << "VTPP After Finished (with exitcode " << vtpp_return_code << ")";
    return 0;
}
