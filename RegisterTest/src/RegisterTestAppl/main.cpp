#include "registertestappl.h"

int main(int argc, char *argv[])
{
    RegisterTestAppl a(argc, argv);
    a.init();
    a.runTest();

    //return a.exec();
}
