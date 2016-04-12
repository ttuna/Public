#include "a.h"
#include <QString>

A::A()
{
    m_p_string = new QString("Name");
}

QString A::getString() const
{
    if (m_p_string == nullptr) return QString();
    return *m_p_string;
}

void A::setString(const QString &in_string)
{
    if (m_p_string == nullptr) return;
    *m_p_string = in_string;
}

