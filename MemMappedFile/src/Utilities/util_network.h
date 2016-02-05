#ifndef UTIL_NETWORK_H
#define UTIL_NETWORK_H

#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include <QHostAddress>
#include <QHostInfo>

class NetworkTools
{
public:
    NetworkTools();

    static QHostInfo getHostInfo(const QString &in_name = QString());
    static QNetworkConfiguration getNetworkConfiguration(const QNetworkConfiguration::BearerType &in_bearer_type = QNetworkConfiguration::BearerUnknown);
    static QNetworkInterface getNetworkInterface(const QString &in_name);
    static QHostAddress getHostAddress(const QNetworkConfiguration &in_network_config);
    static QHostAddress getHostAddress(const QHostInfo &in_host_info);
    static QHostAddress getHostAddress(const QNetworkInterface &in_interface);
};

#endif // UTIL_NETWORK_H
