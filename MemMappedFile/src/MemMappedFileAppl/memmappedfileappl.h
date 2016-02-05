#ifndef MEMMAPPEDFILEAPPL_H
#define MEMMAPPEDFILEAPPL_H

#include <QObject>
#include <QFile>
#include <QCoreApplication>

class MemMappedFileAppl : public QCoreApplication
{
    Q_OBJECT
public:
    MemMappedFileAppl(int &argc, char **argv);

    bool init();
    bool isValid();

    void createFile();
    void mapFile();
    void unmapFile();
    void transformFile();

private:
    bool m_initialized = false;
    QFile m_file;
    uchar* m_mapped_data_1 = nullptr;
    uchar* m_mapped_data_2 = nullptr;
};

#endif // MEMMAPPEDFILEAPPL_H
