#include "c.h"
#include "b.h"

//#include <QString>

C::C()
{
    m_p_b1 = new B();
    m_p_b2 = new B();
}

bool C::equal() const
{
    if (m_p_b1 == nullptr || m_p_b2 == nullptr) return false;

    bool ret = true;
    ret &= (m_p_b2->getAString() == m_p_b1->getAString());
    ret &= (m_p_b2->getInt() == m_p_b1->getInt());
    return ret;
}
