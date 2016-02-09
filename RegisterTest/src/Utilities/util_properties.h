#ifndef UTIL_PROPERTIES_H
#define UTIL_PROPERTIES_H

#include <QObject>
#include <QMetaProperty>
#include <QVariant>
#include <QDebug>
#include <QJsonObject>
#include "util_properties.h"
#include "util_json.h"

// ----------------------------------------------------------------------------
//
// PropertyAccessorCheck
//
// ----------------------------------------------------------------------------
class PropertyCheck
{
    friend class PropertyAccessor;
public:
    typedef enum {
        PROPERTY_ACCESSOR_OPERATOR_UNKNOWN = 0,
        PROPERTY_ACCESSOR_OPERATOR_LOWER,
        PROPERTY_ACCESSOR_OPERATOR_LEQ,
        PROPERTY_ACCESSOR_OPERATOR_EQUAL,
        PROPERTY_ACCESSOR_OPERATOR_GEQ,
        PROPERTY_ACCESSOR_OPERATOR_GREATER,
        PROPERTY_ACCESSOR_OPERATOR_LAST
    } CheckOperator;

    // ----------------------------------------------------------------------------
    // ctor / dtor ...
    // ----------------------------------------------------------------------------
    explicit PropertyCheck() :
        m_value(0), m_operator(PROPERTY_ACCESSOR_OPERATOR_UNKNOWN), m_message(QLatin1String("Not initialized"))
    {}

    explicit PropertyCheck(QVariant in_value, CheckOperator in_op, QString in_msg) :
        m_value(in_value), m_operator(in_op), m_message(in_msg)
    {}

    PropertyCheck(const PropertyCheck& in_other) :
        m_value(in_other.m_value),
        m_operator(in_other.m_operator),
        m_message(in_other.m_message)
    {}

    PropertyCheck& operator=(PropertyCheck in_other)
    {
        std::swap( m_value, in_other.m_value );
        std::swap( m_operator, in_other.m_operator );
        std::swap( m_message, in_other.m_message );

        return *this;
    }

    // ----------------------------------------------------------------------------
    // eval
    // ----------------------------------------------------------------------------
    bool eval(const QVariant& in_value)
    {
        if (in_value.isValid() == false) return false;
        if (in_value.type() != m_value.type()) return false;

        switch(m_operator)
        {
        case PROPERTY_ACCESSOR_OPERATOR_LOWER:
            if (in_value < m_value) return true;
            qDebug() << m_message;
            return false;
            break;
        case PROPERTY_ACCESSOR_OPERATOR_LEQ:
            if (in_value <= m_value) return true;
            qDebug() << m_message;
            return false;
            break;
        case PROPERTY_ACCESSOR_OPERATOR_EQUAL:
            if (in_value == m_value) return true;
            qDebug() << m_message;
            return false;
            break;
        case PROPERTY_ACCESSOR_OPERATOR_GEQ:
            if (in_value >= m_value) return true;
            qDebug() << m_message;
            return false;
            break;
        case PROPERTY_ACCESSOR_OPERATOR_GREATER:
            if (in_value > m_value) return true;
            qDebug() << m_message;
            return false;
            break;
        default:
            qDebug() << "PropertyCheck::eval - Unknown operator";
            return false;
            break;
        }
    }

private:
    QVariant m_value;
    CheckOperator m_operator;
    QString m_message;
};

// ----------------------------------------------------------------------------
// typedef
// ----------------------------------------------------------------------------
typedef QMap<QLatin1String, PropertyCheck> PropertyCheckList;



// ----------------------------------------------------------------------------
//
// PropertyAccessor
//
// ----------------------------------------------------------------------------
class PropertyAccessor : public QObject
{
    Q_OBJECT
public:
    explicit PropertyAccessor(QObject *parent) : QObject(parent)
    {}

    // !!!QVariantMap can easily be converted into QJsonObject for storing and restoring properties ...

    // ----------------------------------------------------------------------------
    // Set
    // ----------------------------------------------------------------------------
    template<typename T> static bool Set(T& in_obj, const QVariantMap& in_props)
    {
        if (in_props.isEmpty()) return true;

        bool ret = true;

        QString prop_name;
        QVariant prop_value;
        for (int i=0; i<in_props.size(); ++i)
        {
            prop_name = in_props.keys().at(i);
            if (prop_name.isEmpty()) continue;

            prop_value = in_props.value(prop_name);
            if (prop_value.isValid() == false) continue;

            ret &= in_obj.setProperty(prop_name.toUtf8(), prop_value);
        }

        return ret;
    }

    // ----------------------------------------------------------------------------
    // Get
    // ----------------------------------------------------------------------------
    template<typename T> static const QVariantMap Get(const T& in_obj, const bool in_recursive = false)
    {
        const QMetaObject* meta_object = in_obj.metaObject();
        if (meta_object == 0) return QVariantMap();

        QVariantMap prop_map;
        prop_map = GetPropertyVariantMap(&in_obj, meta_object, in_recursive);

        return prop_map;
    }


    // ----------------------------------------------------------------------------
    // GetNew ... works :-)
    // ----------------------------------------------------------------------------
    static const QVariantMap GetNew(const QObject* in_obj, const bool in_recursive = false)
    {
        const QMetaObject* meta_object = in_obj->metaObject();
        if (meta_object == 0) return QVariantMap();

        QVariantMap prop_map;
        prop_map = GetPropertyVariantMap(in_obj, meta_object, in_recursive);

        return prop_map;
    }

    // ----------------------------------------------------------------------------
    // Check
    // ----------------------------------------------------------------------------
    template<typename T> static bool Check(T& in_obj, const PropertyCheckList& in_check_list)
    {
        Q_UNUSED(in_obj)
        Q_UNUSED(in_check_list)

        if (in_check_list.isEmpty()) return true;

        // prop_name;
        QMetaProperty prop;
        PropertyCheck check;
        bool ret = true;
        foreach(QLatin1String prop_name, in_check_list.keys())
        {
            prop = GetProperty(in_obj, prop_name);
            if (prop.isValid() == false) continue;
            if (prop.isReadable() == false) continue;

            check = in_check_list[prop_name];
            ret &= check.eval(prop.read(&in_obj));
        }

        return ret;
    }

    // ----------------------------------------------------------------------------
    // GetProperty
    // ----------------------------------------------------------------------------
    template<typename T> static QMetaProperty GetProperty(T& in_obj, QString in_name)
    {
        const QMetaObject* meta_object = &in_obj.staticMetaObject;
        if (meta_object == 0) return QMetaProperty();

        int prop_count = meta_object->propertyCount();
        if (prop_count == 0) return QMetaProperty();

        QMetaProperty meta_prop;
        for (int i=0; i<prop_count; ++i)
        {
            meta_prop = meta_object->property(i);
            if (in_name != QLatin1String(meta_prop.name())) continue;

            return meta_prop;
        }
        return QMetaProperty();
    }

private:
    // ----------------------------------------------------------------------------
    // GetProperty
    // ----------------------------------------------------------------------------
    static QVariantMap GetPropertyVariantMap(const QObject* in_obj, const QMetaObject* in_meta_obj, const bool in_recursive)
    {
        if (in_obj == nullptr) return QVariantMap();
        if (in_meta_obj == nullptr) return QVariantMap();

        int prop_count = in_meta_obj->propertyCount();
        if (prop_count == 0) return QVariantMap();

        QVariantMap prop_map;
        QMetaProperty meta_prop;
        QString prop_name;
        for (int i=0; i<prop_count; ++i)
        {
            meta_prop = in_meta_obj->property(i);
            prop_name = QString::fromLatin1(meta_prop.name());
            if (prop_map.contains(prop_name) == true) continue;

            qDebug() << "meta_prop:" << meta_prop.type() << meta_prop.userType() << meta_prop.typeName();

            if (meta_prop.type() >= QVariant::UserType && in_recursive == true)
            {
                const QMetaObject* sub_meta_object = QMetaType::metaObjectForType(meta_prop.userType());
                if (sub_meta_object == 0) continue;
                qDebug() << "sub_meta_object:" << sub_meta_object->className();

                QVariant sub_obj_var = meta_prop.read(in_obj);
                if (sub_obj_var.isValid() == false) continue;
                qDebug() << "sub_obj_var type:" << sub_obj_var.type();

                QObject* sub_obj = qvariant_cast<QObject *>(sub_obj_var);
                if (sub_obj == nullptr) continue;

                QVariantMap sub_map;
                sub_map = GetPropertyVariantMap(sub_obj, sub_meta_object, in_recursive);
                prop_map[prop_name] = sub_map;
            }
            else if (meta_prop.isReadable())
            {
                prop_map[prop_name] = meta_prop.read(in_obj);
                //prop_map[prop_name] = prop_name;
            }
        }

        return prop_map;
    }
};



// ----------------------------------------------------------------------------
//
// PropertyPersistor
//
// ----------------------------------------------------------------------------
class PropertyPersistor : public QObject
{
    Q_OBJECT
public:
    explicit PropertyPersistor(QObject* parent = 0);

    // ----------------------------------------------------------------------------
    // toFile
    // ----------------------------------------------------------------------------
    template<typename T> static void toFile(const T& in_obj, const QString &in_file_name, const bool in_binary = false)
    {
        if (in_file_name.size() == 0) return;

        // get properties map ...
        QVariantMap prop_map = PropertyAccessor::Get<T>(in_obj, true);
        if (prop_map.isEmpty()) return;

        qDebug() << "toFile:" << prop_map;

        // create json obj from properties map ...
        QJsonObject prop_json_obj(QJsonObject::fromVariantMap(prop_map));
        if (prop_json_obj.isEmpty()) return;

        // get key (name) ...
        QByteArray name(in_obj.objectName().toUtf8());
        if (name.size() == 0) name = in_obj.metaObject()->className();

        JsonFile::JsonFileFlags flags = JsonFile::JSONFILEFLAGS_NONE;
        if (in_binary == true) flags &= JsonFile::JSONFILEFLAGS_BINARY;

        JsonFile::write(in_file_name, QJsonValue(prop_json_obj), QLatin1String(name), flags);
    }

    // ----------------------------------------------------------------------------
    // fromFile
    // ----------------------------------------------------------------------------
    static QVariantMap fromFile(const QObject &in_obj, const QString& in_file_name, const bool in_binary = false)
    {
        Q_UNUSED(in_obj)
        Q_UNUSED(in_file_name)
        Q_UNUSED(in_binary)
        return QVariantMap();
    }
};

#endif // UTIL_PROPERTIES_H
