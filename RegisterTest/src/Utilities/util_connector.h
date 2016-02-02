#ifndef UTIL_CONNECTOR_H
#define UTIL_CONNECTOR_H

#include <QObject>
#include <QMetaObject>
#include <QMetaMethod>
#include <QDebug>
#include <QtAlgorithms>
#include "util_global.h"

// ----------------------------------------------------------------------------
//
// Enums
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ConnectorType
// ----------------------------------------------------------------------------
typedef enum CONNECTOR_TYPE
{
    CONNECTOR_TYPE_UNKNOWN = 0,
    CONNECTOR_TYPE_NEW_BEACON,
    CONNECTOR_TYPE_REGISTER_SERVICE,
    CONNECTOR_TYPE_UNREGISTER_SERVICE,
    CONNECTOR_TYPE_SERVICE_REQUEST,
    CONNECTOR_TYPE_LAST
}ConnectorType;

namespace {
// ----------------------------------------------------------------------------
//
// Participant
//
// ----------------------------------------------------------------------------
class Participant
{
    //Q_OBJECT
    friend class Connector;
public:

    // ----------------------------------------------------------------------------
    // dtor
    // ----------------------------------------------------------------------------
    virtual ~Participant() {}

    // ----------------------------------------------------------------------------
    // object
    // ----------------------------------------------------------------------------
    QObject *object()
    {
        return m_p_object;
    }

    // ----------------------------------------------------------------------------
    // methodName
    // ----------------------------------------------------------------------------
    QByteArray methodName()
    {
        QByteArray method_name = m_method_name.data();
        if (!method_name.contains('('))
            method_name.append("()");

        return method_name;
    }

    // ----------------------------------------------------------------------------
    // connected
    // ----------------------------------------------------------------------------
    uint connected()
    {
        return m_connected;
    }

private:
    explicit Participant(QObject *in_object, QLatin1String in_method_name, QObject *parent = 0) :
        m_connected(0), m_p_object(in_object), m_method_name(in_method_name) {}
    Participant();
    Participant(Participant const&);
    Participant& operator=(Participant const&);

    uint m_connected;
    QObject *m_p_object;
    QLatin1String m_method_name;
};
} //anonymous namespace ...

// ----------------------------------------------------------------------------
//
// Connector
//
// ----------------------------------------------------------------------------
class Connector
{
public:
    typedef enum CONNECTIONSTATUS : int
    {
        CONNECTIONSTATUS_ERROR = -1,
        CONNECTIONSTATUS_OK,
        CONNECTIONSTATUS_NO_COUNTER_PART,
        CONNECTIONSTATUS_CONNECTED,
        CONNECTIONSTATUS_LAST
    }ConnectionStatus;

    // ----------------------------------------------------------------------------
    // ctor / dtor ...
    // ----------------------------------------------------------------------------
    explicit Connector()
    {}

    virtual ~Connector()
    {
        qDeleteAll(m_signal_bridge_map);
        qDeleteAll(m_remote_slot_map);
        qDeleteAll(m_callback_map);
    }

    // ----------------------------------------------------------------------------
    // AddSignalBridge
    // ----------------------------------------------------------------------------
    ConnectionStatus AddSignalBridge(const ConnectorType in_type, QObject *in_sender, QLatin1String in_method_name)
    {
        if (in_sender == 0) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);
        if (in_method_name.size() == 0) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);

        Participant* par = new Participant(in_sender, in_method_name);
        if (par == 0) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);

        qDebug() << "Connector::AddSignalBridge - Add signal bridge:" << in_sender->metaObject()->className() << in_method_name;

        m_signal_bridge_map.insertMulti(in_type, par);

        return CheckConnection(in_type, par, 0);
    }

    // ----------------------------------------------------------------------------
    // AddRemoteSlot
    // ----------------------------------------------------------------------------
    ConnectionStatus AddRemoteSlot(const ConnectorType in_type, QObject *in_receiver, QLatin1String in_method_name)
    {
        if (in_receiver == 0) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);
        if (in_method_name.size() == 0) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);

        Participant* par = new Participant(in_receiver, in_method_name);
        if (par == 0) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);

        qDebug() << "Connector::AddRemoteSlot - Add remote slot:"  << in_receiver->metaObject()->className() << in_method_name;

        m_remote_slot_map.insertMulti(in_type, par);

        return CheckConnection(in_type, 0, par);
    }

    // ----------------------------------------------------------------------------
    // AddCallback
    // ----------------------------------------------------------------------------
    ConnectionStatus AddCallback(const ConnectorType in_type, QObject *in_callback_obj, QLatin1String in_method_name)
    {
        if (in_callback_obj == 0) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);
        if (in_method_name.size() == 0) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);

        Participant* par = new Participant(in_callback_obj, in_method_name);
        if (par == 0) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);

        qDebug() << "Connector::AddCallback - Add callback:"  << in_callback_obj->metaObject()->className() << in_method_name;

        m_callback_map.insertMulti(in_type, par);

        return CONNECTIONSTATUS_OK;
    }

    // ----------------------------------------------------------------------------
    // ExecuteCallback
    // ----------------------------------------------------------------------------
    ConnectionStatus ExecuteCallback(const ConnectorType in_type, const Qt::ConnectionType connectionType,
                                 QGenericReturnArgument& returnValue,
                                 const QGenericArgument& val0 = QGenericArgument( 0 ),
                                 const QGenericArgument& val1 = QGenericArgument(),
                                 const QGenericArgument& val2 = QGenericArgument(),
                                 const QGenericArgument& val3 = QGenericArgument(),
                                 const QGenericArgument& val4 = QGenericArgument(),
                                 const QGenericArgument& val5 = QGenericArgument(),
                                 const QGenericArgument& val6 = QGenericArgument(),
                                 const QGenericArgument& val7 = QGenericArgument(),
                                 const QGenericArgument& val8 = QGenericArgument(),
                                 const QGenericArgument& val9 = QGenericArgument()) const
    {
        QList<Participant*> callback_list = GetCallbacks(in_type);
        if (callback_list.isEmpty()) return CONNECTIONSTATUS_ERROR;

        Participant* par;
        int methodIndex = -1;
        QByteArray method_norm_signature;
        for (int i=0; i<callback_list.size(); ++i)
        {
            par = callback_list.value(i);
            if (par == 0) continue;

            // get QMetaMethod ...
            method_norm_signature = QMetaObject::normalizedSignature(par->methodName());
            methodIndex = par->object()->metaObject()->indexOfMethod(method_norm_signature);
            if (methodIndex == -1) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);
            QMetaMethod callback_method = par->object()->metaObject()->method(methodIndex);
            if (callback_method.isValid() == false) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);

            // execute ...
            callback_method.invoke(par->object(), connectionType, returnValue, val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);
        }

        return CONNECTIONSTATUS_OK;
    }

    // ----------------------------------------------------------------------------
    // ExecuteCallback (w/o return arg)
    // ----------------------------------------------------------------------------
    ConnectionStatus ExecuteCallback(const ConnectorType in_type, const Qt::ConnectionType connectionType,
                                 const QGenericArgument& val0 = QGenericArgument( 0 ),
                                 const QGenericArgument& val1 = QGenericArgument(),
                                 const QGenericArgument& val2 = QGenericArgument(),
                                 const QGenericArgument& val3 = QGenericArgument(),
                                 const QGenericArgument& val4 = QGenericArgument(),
                                 const QGenericArgument& val5 = QGenericArgument(),
                                 const QGenericArgument& val6 = QGenericArgument(),
                                 const QGenericArgument& val7 = QGenericArgument(),
                                 const QGenericArgument& val8 = QGenericArgument(),
                                 const QGenericArgument& val9 = QGenericArgument()) const
    {
        QList<Participant*> callback_list = GetCallbacks(in_type);
        if (callback_list.isEmpty()) return CONNECTIONSTATUS_ERROR;

        Participant* par;
        int methodIndex = -1;
        QByteArray method_norm_signature;
        for (int i=0; i<callback_list.size(); ++i)
        {
            par = callback_list.value(i);
            if (par == 0) continue;

            // get QMetaMethod ...
            method_norm_signature = QMetaObject::normalizedSignature(par->methodName());
            methodIndex = par->object()->metaObject()->indexOfMethod(method_norm_signature);
            if (methodIndex == -1) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);
            QMetaMethod callback_method = par->object()->metaObject()->method(methodIndex);
            if (callback_method.isValid() == false) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);

            // execute ...
            callback_method.invoke(par->object(), connectionType, val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);
        }

        return CONNECTIONSTATUS_OK;
    }

signals:

public slots:

protected:
    // ----------------------------------------------------------------------------
    // CheckConnection
    // ----------------------------------------------------------------------------
    ConnectionStatus CheckConnection(const ConnectorType in_type, Participant* in_sender = 0, Participant* in_receiver = 0)
    {
        QList<Participant*> sender_list;
        QList<Participant*> receiver_list;

        if (in_sender == 0) sender_list = GetSender(in_type);
        else sender_list.append(in_sender);

        if (in_receiver == 0) receiver_list = GetReceiver(in_type);
        else receiver_list.append(in_receiver);

        if (sender_list.isEmpty() || receiver_list.isEmpty())
        {
            qDebug() << "Connector::CheckConnection - type=" << in_type << ": no participants found ...";
            return CONNECTIONSTATUS_NO_COUNTER_PART;
        }

        ConnectionStatus ret = CONNECTIONSTATUS_OK;
        foreach (Participant *sender, sender_list)
        {
            foreach (Participant *receiver,  receiver_list)
            {
                ret = Connect(sender, receiver);
                if (ret == CONNECTIONSTATUS_ERROR) break;
            }
            if (ret == CONNECTIONSTATUS_ERROR) break;
        }

        return ret;
    }

    // ----------------------------------------------------------------------------
    // Connect
    // ----------------------------------------------------------------------------
    ConnectionStatus Connect(Participant *in_sender, Participant *in_receiver)
    {
        if (in_sender == 0 || in_receiver == 0) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);

        int methodIndex = -1;
        QByteArray signal_norm_signature, slot_norm_signature;

        // get signal QMetaMethod ...
        signal_norm_signature = QMetaObject::normalizedSignature(in_sender->methodName());
        methodIndex = in_sender->object()->metaObject()->indexOfSignal(signal_norm_signature);
        if (methodIndex == -1) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);
        QMetaMethod sender_signal = in_sender->object()->metaObject()->method(methodIndex);
        if (sender_signal.isValid() == false) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);

        // get slot QMetaMethod ...
        slot_norm_signature = QMetaObject::normalizedSignature(in_receiver->methodName());
        methodIndex = in_receiver->object()->metaObject()->indexOfMethod(slot_norm_signature);
        if (methodIndex == -1) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);
        QMetaMethod receiver_slot = in_receiver->object()->metaObject()->method(methodIndex);
        if (receiver_slot.isValid() == false) return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);

        // check argument compatibility ...
        if (QMetaObject::checkConnectArgs(signal_norm_signature, slot_norm_signature) == false)
            return assert_return<ConnectionStatus>(CONNECTIONSTATUS_ERROR,_ASSERT_INFO_);

        // connect ...
        bool ret = false;
        if ((ret = QObject::connect(in_sender->object(), sender_signal, in_receiver->object(), receiver_slot)) == true)
        {
            qDebug() << "Connector::Connect - connecting: " << in_sender->object()->metaObject()->className() << "-" << sender_signal.name() << "to" << in_receiver->object()->metaObject()->className() << "-" << receiver_slot.name();
            in_sender->m_connected++;
            in_receiver->m_connected++;
            return CONNECTIONSTATUS_CONNECTED;
        }

        return CONNECTIONSTATUS_OK;
    }

    // ----------------------------------------------------------------------------
    // GetSender
    // ----------------------------------------------------------------------------
    QList<Participant*> GetSender(const ConnectorType in_type) const
    {
        QList<Participant*> sender_list;

        if (m_signal_bridge_map.contains(in_type))
        {
            sender_list = m_signal_bridge_map.values(in_type);
//            if (sender_list.isEmpty() == false)
//                qDebug() << "Connector::GetSender - Sender found ...";
        }
        return sender_list;
    }

    // ----------------------------------------------------------------------------
    // GetReceiver
    // ----------------------------------------------------------------------------
    QList<Participant*> GetReceiver(const ConnectorType in_type) const
    {
        QList<Participant*> receiver_list;

        if (m_remote_slot_map.contains(in_type))
        {
            receiver_list = m_remote_slot_map.values(in_type);
//            if (receiver_list.isEmpty() == false)
//                qDebug() << "Connector::GetReceiver - Receiver found ...";
        }
        return receiver_list;
    }

    // ----------------------------------------------------------------------------
    // GetCallbacks
    // ----------------------------------------------------------------------------
    QList<Participant*> GetCallbacks(const ConnectorType in_type) const
    {
        QList<Participant*> obj_list;

        if (m_callback_map.contains(in_type))
        {
            obj_list = m_callback_map.values(in_type);
//            if (obj_list.isEmpty() == false)
//                qDebug() << "Connector::GetCallbackObj - Callback object found ...";
        }
        return obj_list;
    }

private:
    QMultiMap<ConnectorType, Participant*> m_signal_bridge_map;
    QMultiMap<ConnectorType, Participant*> m_remote_slot_map;
    QMultiMap<ConnectorType, Participant*> m_callback_map;
};

#endif // UTIL_CONNECTOR_H

