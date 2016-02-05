#include "util_network.h"
#include <QDebug>

// ----------------------------------------------------------------------------
// ctor, dtor, ...
// ----------------------------------------------------------------------------
NetworkTools::NetworkTools()
{
}

// ----------------------------------------------------------------------------
// getHostInfo (static)
// ----------------------------------------------------------------------------
QHostInfo NetworkTools::getHostInfo(const QString &in_name)
{
    QHostInfo host_info;

    if (in_name.isEmpty()) host_info = QHostInfo::fromName(QHostInfo::localHostName());
    else host_info = QHostInfo::fromName(in_name);

    if (host_info.error() != QHostInfo::NoError) return QHostInfo();

    return host_info;
}

// ----------------------------------------------------------------------------
// getNetworkConfiguration (static)
// ----------------------------------------------------------------------------
QNetworkConfiguration NetworkTools::getNetworkConfiguration(const QNetworkConfiguration::BearerType &in_bearer_type)
{
    QNetworkConfigurationManager nwc_manager;
    if (in_bearer_type == QNetworkConfiguration::BearerUnknown) return nwc_manager.defaultConfiguration();

    QList<QNetworkConfiguration> nwc_list = nwc_manager.allConfigurations(QNetworkConfiguration::Discovered);
    if (nwc_list.isEmpty()) return QNetworkConfiguration();

    // find interface ...
    QNetworkConfiguration temp, nwc;
    foreach (temp, nwc_list)
    {
        if (temp.bearerType() == in_bearer_type)
        {
            nwc = temp; break;
        }
    }

    return nwc;
}

// ----------------------------------------------------------------------------
// getNetworkInterface (static)
// ----------------------------------------------------------------------------
QNetworkInterface NetworkTools::getNetworkInterface(const QString &in_name)
{
    if (in_name.isEmpty()) return QNetworkInterface();

    QNetworkInterface net_if;
    QList<QNetworkInterface> net_if_list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface temp, net_if_list)
    {
        if (temp.humanReadableName().compare(in_name, Qt::CaseInsensitive) != 0) continue;
        net_if = temp; break;
    }
    return net_if;
}

// ----------------------------------------------------------------------------
// getHostAddress (static)
// ----------------------------------------------------------------------------
QHostAddress NetworkTools::getHostAddress(const QNetworkConfiguration &in_network_config)
{
    if (in_network_config.isValid() == false) return QHostAddress();

    qDebug() << "NetworkTools::getHostAddress - configuration bearer type:" << in_network_config.bearerTypeName();

    QNetworkSession nws(in_network_config);
    if (nws.state() == QNetworkSession::Invalid || nws.state() == QNetworkSession::NotAvailable) return QHostAddress();

    qDebug() << "NetworkTools::getHostAddress - session state:" << nws.state();

    QNetworkInterface nwi = nws.interface();
    if (nwi.isValid() == false) return QHostAddress();
    if (nwi.addressEntries().isEmpty()) return QHostAddress();

    foreach(QNetworkAddressEntry temp, nwi.addressEntries())
        qDebug() << "NetworkTools::getHostAddress - session addr entry:" << temp.ip().toString();

    QNetworkAddressEntry nwae = nwi.addressEntries().first();
    QHostAddress host_address = nwae.ip();

    return host_address;
}

// ----------------------------------------------------------------------------
// getHostAddress (static)
// ----------------------------------------------------------------------------
QHostAddress NetworkTools::getHostAddress(const QHostInfo &in_host_info)
{
    if (in_host_info.lookupId() == -1) return QHostAddress();
    if (in_host_info.error() != QHostInfo::NoError) return QHostAddress();
    if (in_host_info.addresses().isEmpty()) return QHostAddress();

    return in_host_info.addresses().first();
}

// ----------------------------------------------------------------------------
// getHostAddress (static)
// ----------------------------------------------------------------------------
QHostAddress NetworkTools::getHostAddress(const QNetworkInterface &in_interface)
{
    if (in_interface.isValid() == false) return QHostAddress();

    QList<QNetworkAddressEntry> addr_entry_list = in_interface.addressEntries();
    if (addr_entry_list.isEmpty()) return QHostAddress();

    return addr_entry_list.first().ip();
}
