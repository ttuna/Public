#include "util_configfile.h"
#include "util_properties.h"
#include "util_global.h"

// ----------------------------------------------------------------------------
// ctor / dtor ...
// ----------------------------------------------------------------------------
ConfigFile::ConfigFile(QObject* parent) : QObject(parent)
{

}

// ----------------------------------------------------------------------------
// init
// ----------------------------------------------------------------------------
bool ConfigFile::init(const QString &in_file_name, JsonFile::JsonFileFlags in_file_flags)
{
    if (in_file_name.isEmpty()) return assert_return<bool>(false, _ASSERT_INFO_);
    if (QFile::exists(in_file_name) == false) return assert_return<bool>(false, _ASSERT_INFO_);

    m_file_name = in_file_name;
    m_file_flags = in_file_flags;

    m_initialized = true;
    return m_initialized;
}

// ----------------------------------------------------------------------------
// isValid
// ----------------------------------------------------------------------------
bool ConfigFile::isValid() const
{
    return m_initialized;
}

// ----------------------------------------------------------------------------
// read
// ----------------------------------------------------------------------------
QVariantMap ConfigFile::read(const QString& in_entry_name, bool recursive) const
{
    if (m_initialized == false) return assert_return<QVariantMap>(QVariantMap(), _ASSERT_INFO_);

    // try to split entry name into "::" separated tokens ...
    QStringList entry_name_tokens = in_entry_name.split(QLatin1String("::"), QString::SkipEmptyParts);
    if (entry_name_tokens.isEmpty()) entry_name_tokens.append(in_entry_name);

    QJsonDocument json_doc = JsonFile::read(m_file_name, m_file_flags);
    if (json_doc.isNull()) return assert_return<QVariantMap>(QVariantMap(), _ASSERT_INFO_);

    // TODO: should arrays also be possible ???
    QJsonObject json_obj = json_doc.object();
    if (json_obj.isEmpty()) return QVariantMap();

    // each token has to represent a QJsonObject which contains the next token ...
    QString token;
    QJsonValue json_value;
    foreach (token, entry_name_tokens)
    {
        // if there is more than one token allow recursive search only for the first ...
        recursive &= (token == entry_name_tokens.first());
        json_value = JsonHelper::findEntry(json_obj, token, recursive);
        if (json_value.isNull() || json_value.isUndefined()) break;
        if (token != entry_name_tokens.last()) json_obj = json_value.toObject();
        if (json_obj.isEmpty()) break;
    }

    QVariantMap ret;
    if (json_value.isNull() == false || json_value.isUndefined() == false)
        ret[token] = json_value.toVariant();

    return ret;
}

// ----------------------------------------------------------------------------
// write
// ----------------------------------------------------------------------------
void ConfigFile::write(const QVariantMap &in_data_map, const QString& in_entry_name) const
{
    if (m_initialized == false) return assert_return<void>(_ASSERT_INFO_);
    if (in_data_map.isEmpty()) return;

    QJsonDocument file_doc = JsonFile::read(m_file_name, m_file_flags);
    if (file_doc.isNull()) return assert_return<void>(_ASSERT_INFO_);

    // TODO: should arrays also be possible ???
    QJsonObject file_doc_obj = file_doc.object();
    QJsonObject data_obj = QJsonObject::fromVariantMap(in_data_map);
    if (data_obj.isEmpty()) return assert_return<void>(_ASSERT_INFO_);

    qDebug() << "\n";
    qDebug() << "write - befor merge:" << file_doc_obj << "\n";

    // merge data to file by entry name ...
    QString key = in_entry_name; // could be modified during merge ...
    if (merge(file_doc_obj, data_obj, key) == false)
        return assert_return<void>(_ASSERT_INFO_);

    qDebug() << "write - after merge:" << file_doc_obj << "\n";

    QJsonValue value = file_doc_obj;
    if (value.isUndefined()) return assert_return<void>(_ASSERT_INFO_);

    qDebug() << "write" << key << value << "\n";

    // data is already merged with file content - truncate file when opening ...
    JsonFile::write(m_file_name, value, key, m_file_flags | JsonFile::JSONFILEFLAGS_TRUNCATE);
}

// ----------------------------------------------------------------------------
// write
// ----------------------------------------------------------------------------
void ConfigFile::write(const QJsonDocument &in_doc) const
{
    if (m_initialized == false) return assert_return<void>(_ASSERT_INFO_);
    if (in_doc.isEmpty()) return;

    // replace all file content with json doc - truncate file when opening ...
    JsonFile::write(m_file_name, in_doc, m_file_flags | JsonFile::JSONFILEFLAGS_TRUNCATE);
}

// ----------------------------------------------------------------------------
// merge
// ----------------------------------------------------------------------------
bool ConfigFile::merge(QJsonObject& in_dest, const QJsonObject& in_data, QString& in_entry_name) const
{
    if (m_initialized == false) return assert_return<bool>(false, _ASSERT_INFO_);

    // split entry name into "::" separated tokens ...
    QStringList entry_name_tokens = in_entry_name.split(QLatin1String("::"), QString::SkipEmptyParts);
    if (entry_name_tokens.isEmpty()) entry_name_tokens.append(in_entry_name);

    QMap<QString, QJsonObject> results = buildMergeList(in_dest, entry_name_tokens);
    if (results.isEmpty()) return assert_return<bool>(false, _ASSERT_INFO_);

    // get last object from merge results ...
    QString last_token_name = entry_name_tokens.last();
    if (last_token_name.isEmpty()) return assert_return<bool>(false, _ASSERT_INFO_);

    QJsonObject result_obj;
    result_obj = results[last_token_name];

    // insert data and store back to results map ...
    for (auto data_iter = in_data.begin(); data_iter != in_data.end(); ++data_iter)
        result_obj.insert(data_iter.key(), data_iter.value());

    results[last_token_name] = result_obj;

    // merge data into token hierachy ...
    for (auto iter = --entry_name_tokens.end(); iter >= entry_name_tokens.begin(); --iter)
    {
        result_obj = results[*iter];

        if (*iter != last_token_name)
        {
            result_obj.insert(in_entry_name, in_dest);
        }

        in_dest = result_obj;
        in_entry_name = *iter;

        qDebug() << "merge - end of iteration: " << in_entry_name << in_dest;
    }

    // check if old data entry exists and insert new node ...
    // key for old data is an empty string!!!
    result_obj = results[QLatin1String("")];
    if (result_obj.isEmpty() == false)
    {
        result_obj.insert(in_entry_name, in_dest);

        in_dest = result_obj;
        in_entry_name = QLatin1String("");
    }

    qDebug() << "merge - end of merge: " << in_entry_name << in_dest << "\n";

    return true;
}

// ----------------------------------------------------------------------------
// buildMergeList
// ----------------------------------------------------------------------------
QMap<QString, QJsonObject> ConfigFile::buildMergeList(const QJsonObject &in_src, const QStringList &in_tokens) const
{
    if (m_initialized == false) return assert_return<QMap<QString, QJsonObject>>(QMap<QString, QJsonObject>(), _ASSERT_INFO_);
    if (in_src.isEmpty()) return QMap<QString, QJsonObject>();
    if (in_tokens.isEmpty()) return QMap<QString, QJsonObject>();

    QJsonObject search_obj = in_src;
    QMap<QString, QJsonObject> results;
    QJsonValue val;

    // every existing file content has to be added to results to avoid data loss
    // key for existing data is an empty string!!!
    results.insert(QLatin1String(""), in_src);

    foreach (QString key, in_tokens)
    {
        val = JsonHelper::findEntry(search_obj, key, (key == in_tokens.first()));
        if (val.isNull() || val.isUndefined()) val = QJsonValue(QJsonValue::Object); // create empty entry if not found ...
        search_obj = val.toObject();
        results[key] = search_obj;
    }

    return results;
}

// ----------------------------------------------------------------------------
// Propterties ...
// ----------------------------------------------------------------------------
QString ConfigFile::fileName() const
{
    return m_file_name;
}

JsonFile::JsonFileFlags ConfigFile::fileFlags() const
{
    return m_file_flags;
}

void ConfigFile::setFileName(QString file_name)
{
    if (m_file_name == file_name)
        return;

    m_file_name = file_name;
    emit fileNameChanged(file_name);
}

void ConfigFile::setFileFlags(JsonFile::JsonFileFlags file_flags)
{
    if (m_file_flags == file_flags)
        return;

    m_file_flags = file_flags;
    emit fileFlagsChanged(file_flags);
}
