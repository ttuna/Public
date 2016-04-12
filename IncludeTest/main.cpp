#include <QCoreApplication>
#include <QDebug>
#include "c.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    C c;
    qDebug() << c.equal();

    return a.exec();
}
