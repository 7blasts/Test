#include <QCoreApplication>

#include "main.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    gx_time_implementation timer;

    return app.exec();
}
