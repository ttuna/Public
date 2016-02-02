#ifndef TESTPROPERTYOBJECT_H
#define TESTPROPERTYOBJECT_H

#include <QObject>

// ----------------------------------------------------------------------------
//
// class TestPropertyObject
//
// ----------------------------------------------------------------------------
class TestPropertyObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int count READ count WRITE setCount)

public:
    explicit TestPropertyObject(QObject* parent = 0) : QObject(parent), m_name(QLatin1String("Test")), m_count(100)
    {}

    virtual ~TestPropertyObject()
    {}

    // Properties ...
    QString name() const
    {
        return m_name;
    }
    int count() const
    {
        return m_count;
    }

public slots:
    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
    }
    void setCount(int count)
    {
        if (m_count == count)
            return;

        m_count = count;
    }

private:
    QString m_name;
    int m_count = 0;
};

Q_DECLARE_METATYPE(TestPropertyObject*)

#endif // TESTPROPERTYOBJECT_H
