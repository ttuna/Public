#ifndef REGISTERTESTAPPL_H
#define REGISTERTESTAPPL_H

#include <QObject>
#include <QCoreApplication>
#include <QVariant>

class TestPropertyObject;

typedef QList<int> IntList;
Q_DECLARE_METATYPE(IntList)

class RegisterTestAppl : public QCoreApplication
{
    Q_OBJECT
    // TEST ...
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(IntList list READ list WRITE setList NOTIFY listChanged)
    Q_PROPERTY(TestPropertyObject* test READ test WRITE setTest NOTIFY testChanged)
public:
    explicit RegisterTestAppl(int &argc, char **argv);
    ~RegisterTestAppl();

    bool init();
    bool isValid();
    void runTest();

    QString name() const
    {
        return m_name;
    }

    TestPropertyObject* test() const
    {
        return m_test;
    }

    IntList list() const
    {
        return m_list;
    }

public slots:
    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(name);
    }

    void setList(IntList list)
    {
        if (m_list == list)
            return;

        m_list = list;
        emit listChanged(list);
    }

    void setTest(TestPropertyObject* test)
    {
        if (m_test == test)
            return;

        m_test = test;
        emit testChanged(test);
    }

signals:
    void nameChanged(QString name);
    void listChanged(IntList list);
    void testChanged(TestPropertyObject* test);

private:
    bool m_initialized = false;
    QString m_name;
    IntList m_list;
    TestPropertyObject* m_test = nullptr;
};

#endif // REGISTERTESTAPPL_H
