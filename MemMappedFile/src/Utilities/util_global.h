#ifndef UTIL_GLOBAL
#define UTIL_GLOBAL

//#define NDEBUG    // turn assert off - use this before #inlcude<cassert>!

#include <QLatin1String>
#include <QByteArray>
#include <QDebug>
#include <cassert>

#define _FILE_AND_LINE_ ([]()->QByteArray {QByteArray ba; ba.append("file: "); ba.append(__FILE__); ba.append(" line: "); ba.append(QByteArray::number(__LINE__)); return ba;}())
#define _ASSERT_INFO_ QLatin1String(_FILE_AND_LINE_)
#define _ASSERT_RETURN_(x) ([]()->decltype(x){qDebug() << QLatin1String(QByteArray(__FUNCTION__)); assert(false); return x;}())
// ----------------------------------------------------------------------------
// assert_return ...
// ----------------------------------------------------------------------------
template<typename T> inline T assert_return(T in_ret_value, const QLatin1String& in_debug_out = QLatin1String(""))
{
    if (in_debug_out.size() != 0)
        qDebug() << "\n" << "ASSERT RETURN:" << in_debug_out << "\n";

    assert(false);
    return in_ret_value;
}
// ----------------------------------------------------------------------------
template<typename T> inline void assert_return(const QLatin1String& in_debug_out = QLatin1String(""))
{
    if (in_debug_out.size() != 0)
        qDebug() << "\n" << "ASSERT RETURN:" << in_debug_out << "\n";

    assert(false);
    return;
}

#endif // UTIL_GLOBAL
