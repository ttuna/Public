#include "util_json.h"
#include <QDebug>

// ----------------------------------------------------------------------------
//
// JsonFile
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// write
// ----------------------------------------------------------------------------
void JsonFile::write(const QString &in_file_name, const QJsonDocument &in_json_doc, const JsonFileFlags &in_flags)
{
    QJsonObject obj = in_json_doc.object();
    write(in_file_name, obj, QLatin1String(""), in_flags);
}

// ----------------------------------------------------------------------------
// write
// ----------------------------------------------------------------------------
void JsonFile::write(const QString &in_file_name, const QJsonValue &in_json_value, const QString &in_entry_name, const JsonFileFlags &in_flags)
{
    if (in_file_name.size() == 0) return;

    QFile json_file(in_file_name);
    QJsonDocument json_file_doc;

    if (prepare(json_file, QIODevice::ReadWrite, in_flags, json_file_doc) == false) return;

    // update json doc ...
    QJsonObject json_obj;
    if (in_entry_name.isEmpty() == false)
    {
        // if in_entry_name isn't empty insert new value ...
        json_obj = json_file_doc.object();
        json_obj.insert(in_entry_name, in_json_value);
    }
    else
    {
        // if in_entry_name is empty set in_json_value as content object ...
        json_obj = in_json_value.toObject();
    }

    json_file_doc.setObject(json_obj);

    // json doc to file ...
    QByteArray file_data = (in_flags & JSONFILEFLAGS_BINARY) ? json_file_doc.toBinaryData() : json_file_doc.toJson();
    json_file.resize(0);
    json_file.write(file_data);
    json_file.close();
}

// ----------------------------------------------------------------------------
// read
// ----------------------------------------------------------------------------
QJsonDocument JsonFile::read(const QString &in_file_name, const JsonFileFlags &in_flags)
{
    if (in_file_name.isEmpty()) return QJsonDocument();

    QFile json_file(in_file_name);
    if (json_file.exists() == false) return QJsonDocument();
    QJsonDocument json_file_doc;

    if (prepare(json_file, QIODevice::ReadOnly, in_flags, json_file_doc) == false) return QJsonDocument();

    json_file.close();
    return json_file_doc;
}

// ----------------------------------------------------------------------------
// read
// ----------------------------------------------------------------------------
QJsonValue JsonFile::read(const QString &in_file_name, const QString &in_entry_name, const JsonFileFlags &in_flags)
{
    if (in_file_name.isEmpty()) return QJsonValue();

    QJsonDocument json_file_doc = read(in_file_name, in_flags);
    if (json_file_doc.isEmpty()) return QJsonValue();

    QJsonObject json_object = json_file_doc.object();
    if (json_object.isEmpty()) return QJsonValue();

    if (in_entry_name.isEmpty()) return QJsonValue(json_object);

    return JsonHelper::findEntry(json_object, in_entry_name, in_flags.testFlag(JSONFILEFLAGS_RECURSIVE_ENTRY_SEARCH));
}

// ----------------------------------------------------------------------------
// prepare
// ----------------------------------------------------------------------------
bool JsonFile::prepare(const QFile& in_file, const QIODevice::OpenMode &in_mode, const JsonFileFlags &in_flags, QJsonDocument& out_js_doc)
{
    QIODevice::OpenMode open_mode = in_mode;
    if ((in_flags & JSONFILEFLAGS_BINARY) == false) open_mode |= QIODevice::Text;
    if (in_flags & JSONFILEFLAGS_APPEND) open_mode |= QIODevice::Append;
    if (in_flags & JSONFILEFLAGS_TRUNCATE) open_mode |= QIODevice::Truncate;

    // read data from file ...
    QFile &json_file = const_cast<QFile&>(in_file);
    if (json_file.isOpen()) json_file.close();
    if (json_file.open(open_mode) == false) return false;
    QByteArray file_data = json_file.readAll();

    // create json doc from file data ...
    QJsonDocument json_file_doc;
    if (file_data.size() > 0)
    {
        if ((in_flags.testFlag(JSONFILEFLAGS_BINARY)) == false)
        {
            QJsonParseError error;
            json_file_doc = QJsonDocument::fromJson(file_data, &error);
            if (error.error != QJsonParseError::NoError)
            {
                qDebug() << "\nFile:" << json_file.fileName() << "- QJsonParseError:" << error.errorString();
                return false;
            }
        }
        else
        {
            json_file_doc = QJsonDocument::fromBinaryData(file_data);
        }
        if (json_file_doc.isNull()) return false;
    }

    out_js_doc = json_file_doc;
    return true;
}



// ----------------------------------------------------------------------------
//
// JsonHelper
//
// ----------------------------------------------------------------------------
const QJsonObject JsonHelper::m_dummy = QJsonObject();
// ----------------------------------------------------------------------------
// findEntry (static)
// ----------------------------------------------------------------------------
QJsonValue JsonHelper::findEntry(const QJsonObject &in_obj, const QString& in_entry_name, bool in_recursive)
{
    if (in_obj.isEmpty()) return QJsonValue();
    if (in_entry_name.isEmpty()) return QJsonValue();

    auto iter = in_obj.find(in_entry_name);
    if (iter == in_obj.end() && in_recursive == false) return QJsonObject();

    QJsonValue json_value = iter.value();
    if (json_value.isNull() == false && json_value.isUndefined() == false) return json_value;

    QJsonObject child_obj = {};
    QJsonValue ret = {};
    for (iter = in_obj.begin(); iter != in_obj.end(); ++iter)
    {
        json_value = *iter;
        if (json_value.isNull() || json_value.isUndefined() || json_value.isObject()==false) continue;

        child_obj = json_value.toObject();
        if (child_obj.isEmpty()) continue;

        ret = findEntry(child_obj, in_entry_name, in_recursive);
        if (ret.isNull() == false && ret.isUndefined() == false) break;
    }

    return ret;
}

// ----------------------------------------------------------------------------
// findEntry (static)
// ----------------------------------------------------------------------------
QJsonValueRef JsonHelper::findEntry(QJsonObject &in_obj, const QString& in_entry_name)
{
    if (in_obj.isEmpty()) return QJsonValueRef(&in_obj, 0);
    if (in_entry_name.isEmpty()) return QJsonValueRef(&in_obj, 0);

    auto iter = in_obj.find(in_entry_name);
    return *iter;
}
