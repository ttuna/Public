#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::string input_data;
    std::getline(std::cin, input_data);

    std::cout << "Hello World from ClientProcess" << std::endl;
    std::cout << "Echo:" << input_data << std::endl;
    std::cerr << "CErr from ClientProcess" << std::endl;

    return a.exec();
}
