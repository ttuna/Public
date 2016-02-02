#ifndef UTIL_JSON_H
#define UTIL_JSON_H

#include <QObject>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

// ----------------------------------------------------------------------------
//
// JsonFile
//
// ----------------------------------------------------------------------------
class JsonFile
{
public:
    enum JFFlags
    {
        JSONFILEFLAGS_NONE = 0x0000,
        JSONFILEFLAGS_BINARY = 0x0001,
        JSONFILEFLAGS_APPEND = 0x0002,
        JSONFILEFLAGS_TRUNCATE = 0x0004,
        JSONFILEFLAGS_RECURSIVE_ENTRY_SEARCH = 0x0008,
    };
    Q_DECLARE_FLAGS(JsonFileFlags, JFFlags)

    virtual ~JsonFile() {}

    static void write(const QString &in_file_name, const QJsonDocument& in_json_doc, const JsonFileFlags &in_flags);
    static void write(const QString &in_file_name, const QJsonValue &in_json_value, const QString &in_entry_name, const JsonFileFlags &in_flags);
    static QJsonDocument read(const QString &in_file_name, const JsonFileFlags &in_flags);
    static QJsonValue read(const QString &in_file_name, const QString &in_entry_name, const JsonFileFlags &in_flags);

private:
    explicit JsonFile() {}
    static bool prepare(const QFile& in_file, const QIODevice::OpenMode &in_mode, const JsonFileFlags &in_flags, QJsonDocument& out_js_doc);
};

// ----------------------------------------------------------------------------
//
// JsonHelper
//
// ----------------------------------------------------------------------------
class JsonHelper
{
public:
    virtual ~JsonHelper() {}

    static QJsonValue findEntry(const QJsonObject& in_obj, const QString &in_entry_name, bool in_recursive = false);
    static QJsonValueRef findEntry(QJsonObject &in_obj, const QString &in_entry_name);

private:
    explicit JsonHelper() {}
    const static QJsonObject m_dummy;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(JsonFile::JsonFileFlags)  // create operator|()...

#endif // UTIL_JSON_H
