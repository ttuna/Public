#include "memmappedfileappl.h"
#include <QDebug>

const quint32 MAPPING_SIZE = 1024;
const QString FILE_NAME = QLatin1String("test.dat");

// ----------------------------------------------------------------------------
// ctor / dtor ...
// ----------------------------------------------------------------------------
MemMappedFileAppl::MemMappedFileAppl(int &argc, char **argv) : QCoreApplication(argc, argv)
{

}

// ----------------------------------------------------------------------------
// init
// ----------------------------------------------------------------------------
bool MemMappedFileAppl::init()
{
    m_file.setFileName(FILE_NAME);
    m_initialized = true;
    return m_initialized;
}

// ----------------------------------------------------------------------------
// isValid
// ----------------------------------------------------------------------------
bool MemMappedFileAppl::isValid()
{
    return m_initialized;
}

// ----------------------------------------------------------------------------
// createFile
// ----------------------------------------------------------------------------
void MemMappedFileAppl::createFile()
{
    if (m_initialized == false) return;

    if (m_file.open(QIODevice::ReadWrite | QIODevice::Truncate) == false) return;

    // create BIG file ...
    for (quint32 i = 0; i < 2048; ++i)
    {
        for (quint32 j = 0; j < 2048; ++j)
        {
            m_file.write(QByteArray(QLatin1String("0").data()));
        }
    }

    m_file.close();
}

// ----------------------------------------------------------------------------
// mapFile
// ----------------------------------------------------------------------------
void MemMappedFileAppl::mapFile()
{
    if (m_initialized == false) return;
    if (m_mapped_data_1 != nullptr) return;
    if (m_mapped_data_2 != nullptr) return;

    if (m_file.open(QIODevice::ReadWrite) == false) return;

    m_mapped_data_1 = m_file.map(0,MAPPING_SIZE);               // 0000 ... 1023
    m_mapped_data_2 = m_file.map(MAPPING_SIZE,MAPPING_SIZE);    // 1024 ... 2047

    qDebug() << "m_mapped_data_1:" << m_mapped_data_1;
    qDebug() << "m_mapped_data_2:" << m_mapped_data_2;

    m_file.close();
}

// ----------------------------------------------------------------------------
// unmapFilet
// ----------------------------------------------------------------------------
void MemMappedFileAppl::unmapFile()
{
    if (m_initialized == false) return;
    if (m_mapped_data_1 == nullptr) return;
    if (m_mapped_data_2 == nullptr) return;

    if (m_file.unmap(m_mapped_data_1) != false)
        m_mapped_data_1 = nullptr;

    if (m_file.unmap(m_mapped_data_2) != false)
        m_mapped_data_2 = nullptr;
}

// ----------------------------------------------------------------------------
// transformFile
// ----------------------------------------------------------------------------
void MemMappedFileAppl::transformFile()
{
    if (m_initialized == false) return;
    if (m_mapped_data_1 == nullptr) return;
    if (m_mapped_data_2 == nullptr) return;

    QByteArray data;

    data = QByteArray::fromRawData((char*)m_mapped_data_1, MAPPING_SIZE);
    std::for_each(data.begin(),data.end(), [](char &n){ n = '1';});
    memcpy(m_mapped_data_1, data.data(), MAPPING_SIZE); // copy back to mapped file mem ...

    data = QByteArray::fromRawData((char*)m_mapped_data_2, MAPPING_SIZE);
    std::for_each(data.begin(),data.end(), [](char &n){ n = '2';});
    memcpy(m_mapped_data_2, data.data(), MAPPING_SIZE); // copy back to mapped file mem ...
}
