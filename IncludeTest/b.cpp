#include "b.h"
#include "a.h"
#include <QString>

B::B()
{
    m_p_a = new A();
}

QString B::getAString() const
{
    if (m_p_a == nullptr) return QString();
    return m_p_a->getString();
}

int B::getInt() const
{
    return m_int;
}
