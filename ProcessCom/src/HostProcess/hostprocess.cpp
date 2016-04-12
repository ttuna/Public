#include "hostprocess.h"
#include <QDebug>

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
HostProcess::HostProcess(QObject *parent) : QObject(parent)
{
}

HostProcess::~HostProcess()
{
    if (m_process != nullptr)
    {
        m_process->closeWriteChannel();

        if (m_process->state() == QProcess::Running)
            m_process->kill();  // the hard way ...

        delete m_process;
        m_process = nullptr;
    }
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
bool HostProcess::init()
{
    m_initialized = true;
    return m_initialized;
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
bool HostProcess::isValid()
{
    return m_initialized;
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
bool HostProcess::launch(const QString &in_executable, const QStringList &in_args)
{
    if (m_initialized == false) return false;
    if (in_executable.isEmpty()) return false;
    if (m_process != nullptr) return false;

    QFileInfo file_info(in_executable);
    if (file_info.isExecutable() == false) return false;

    m_process = new QProcess;
    if (m_process == nullptr) return false;

    qDebug() << "HostProcess::launch: new process ==" << m_process;

    connect(m_process, &QProcess::readyReadStandardOutput, this, &HostProcess::readProcessStandardOutput);
    connect(m_process, &QProcess::readyRead, this, &HostProcess::readProcessOutput);

    m_process->start(in_executable, in_args);
    if (!m_process->waitForStarted(30000)) return false;

    // allow the process to start up ...
    //QThread::msleep(200);

    return true;
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void HostProcess::write(const QByteArray &in_data)
{
    if (m_initialized == false) return;
    if (in_data.isEmpty()) return;
    if (m_process == nullptr) return;
    if (m_process->state() != QProcess::Running) return;

    qDebug() << "HostProcess::write: data ==" << in_data;

    m_process->write(in_data);
    m_process->closeWriteChannel();
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void HostProcess::readProcessStandardOutput()
{
    if (m_initialized == false) return;

    m_data = m_process->readAllStandardOutput();
    qDebug() << "HostProcess::readProcessStandardOutput: data ==" << m_data;
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void HostProcess::readProcessOutput()
{
    if (m_initialized == false) return;

    m_data = m_process->readAll();
    qDebug() << "HostProcess::readProcessOutput: data ==" << m_data;
}
