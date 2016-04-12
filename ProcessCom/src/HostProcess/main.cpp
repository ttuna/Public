#include <QCoreApplication>
#include "hostprocess.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HostProcess host_proc;
    host_proc.init();

    if (host_proc.launch(QLatin1String("ClientProcess.exe")) == false) return -1;
    host_proc.write(QByteArray("test data ..."));

    return a.exec();
}
