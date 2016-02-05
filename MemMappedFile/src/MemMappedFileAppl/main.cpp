#include "memmappedfileappl.h"
#include <conio.h>
#include <Windows.h>
#include <QDebug>

// ----------------------------------------------------------------------------
// select
// ----------------------------------------------------------------------------
int select(MemMappedFileAppl& a)
{
    QLatin1String menu_txt("'c' = create data file; 'm' = map file; 't' = transform data; 'u'= unmap file; 'x' = exit");

    qDebug() << "\n\n" << menu_txt;

    bool cont = true;
    while(cont)
    {
        if(_kbhit())
        {
            char c = _getch();
            switch(c)
            {
                case 'c':
                {
                    qDebug() << "create data file...";
                    a.createFile();
                    qDebug() << "\n\n" << menu_txt;
                    cont = true;    // stay in loop ...
                } break;
                case 'm':
                {
                    qDebug() << "map file ...";
                    a.mapFile();
                    qDebug() << "\n\n" << menu_txt;
                    cont = true;    // stay in loop ...
                } break;
                case 't':
                {
                    qDebug() << "transform data ...";
                    a.transformFile();
                    qDebug() << "\n\n" << menu_txt;
                    cont = true;    // stay in loop ...
                } break;
                case 'u':
                {
                    qDebug() << "unmap file ...";
                    a.unmapFile();
                    qDebug() << "\n\n" << menu_txt;
                    cont = true;    // stay in loop ...
                } break;
                case 'x':
                {
                    qDebug() << "\nexit ...";
                    return(-1);
                } break;
                default:
                {
                    qDebug() << "\n\n" << menu_txt;
                    cont = true;    // stay in loop ...
                }
            }
        }
        Sleep(10);
    }
    return 0;
}

// ----------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    MemMappedFileAppl a(argc, argv);
    a.init();

    if (select(a) < 0) exit(-1);

    return a.exec();
}
