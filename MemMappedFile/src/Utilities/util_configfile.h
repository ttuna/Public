#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QObject>
#include <QVariant>
#include <QHash>
#include "util_json.h"
#include "util_properties.h"


typedef QHash<QString, QString> KeyMap;

class ConfigFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString file_name READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(JsonFile::JsonFileFlags file_flags READ fileFlags WRITE setFileFlags NOTIFY fileFlagsChanged)
public:
    explicit ConfigFile(QObject *parent = nullptr);
    bool init(const QString& in_file_name, JsonFile::JsonFileFlags in_file_flags);
    bool isValid() const;

    // in_entry_name: can have scope operator for representing object hierachy ...
    // e.g.: PSM::port_range::port_range1
    QVariantMap read(const QString &in_entry_name = QString(), bool recursive = false) const;
    void write(const QVariantMap& in_data_map, const QString& in_entry_name) const;
    void write(const QJsonDocument& in_doc) const;

    // properties ...
    QString fileName() const;
    JsonFile::JsonFileFlags fileFlags() const;

public slots:
    // properties ...
    void setFileName(QString file_name);
    void setFileFlags(JsonFile::JsonFileFlags file_flags);

signals:
    // properties ...
    void fileNameChanged(QString file_name);
    void fileFlagsChanged(JsonFile::JsonFileFlags file_flags);

private:
    bool merge(QJsonObject &in_dest, const QJsonObject &in_data, QString &in_entry_name) const;
    QMap<QString, QJsonObject> buildMergeList(const QJsonObject &in_src, const QStringList &in_tokens) const;

    bool m_initialized = false;
    JsonFile::JsonFileFlags m_file_flags = JsonFile::JSONFILEFLAGS_NONE;
    QString m_file_name = {};
};

#endif // CONFIGFILE_H
