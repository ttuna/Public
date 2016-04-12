#ifndef HOSTPROCESS_H
#define HOSTPROCESS_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QFileInfo>
#include <QThread>

// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------
class HostProcess : public QObject
{
    Q_OBJECT
public:
    explicit HostProcess(QObject* parent=nullptr);
    virtual ~HostProcess();

    bool init();
    bool isValid();

public slots:
    bool launch(const QString& in_executable, const QStringList& in_args = QStringList());
    void write(const QByteArray& in_data);

private slots:
    void readProcessStandardOutput();
    void readProcessOutput();

private:
    bool m_initialized = false;
    QProcess* m_process = nullptr;
    QByteArray m_data;
};

#endif // HOSTPROCESS_H
