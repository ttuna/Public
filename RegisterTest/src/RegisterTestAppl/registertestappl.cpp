#include "registertestappl.h"
#include "testpropertyobject.h"
#include "util_properties.h"
#include <QDebug>

// ----------------------------------------------------------------------------
// ctor / dtor ...
// ----------------------------------------------------------------------------
RegisterTestAppl::RegisterTestAppl(int &argc, char **argv) : QCoreApplication(argc, argv)
{

}

RegisterTestAppl::~RegisterTestAppl()
{
    if (m_test != nullptr)
    {
        delete m_test;
        m_test = nullptr;
    }
}

// ----------------------------------------------------------------------------
// init
// ----------------------------------------------------------------------------
bool RegisterTestAppl::init()
{
    int id = 0;

    // property test ...
    m_test = new TestPropertyObject;
    if (m_test == nullptr) return false;

    // swap this calls just for fun ... ;-)
    id = qRegisterMetaType<IntList>("IntList");
    qDebug() << "qRegisterMetaType<IntList>:" << id;
    id = qRegisterMetaType<TestPropertyObject*>("TestPropertyObjectPtr");
    qDebug() << "qRegisterMetaType<TestPropertyObject*>:" << id;

    m_initialized = true;
    return m_initialized;
}

// ----------------------------------------------------------------------------
// isValid
// ----------------------------------------------------------------------------
bool RegisterTestAppl::isValid()
{
    return m_initialized;
}

// ----------------------------------------------------------------------------
// runTest
// ----------------------------------------------------------------------------
void RegisterTestAppl::runTest()
{
    if (m_initialized == false) return;

    //QVariantMap prop_map = PropertyAccessor::Get<RegisterTestAppl>(*this, true);
    QVariantMap prop_map = PropertyAccessor::GetNew(this, true);
    qDebug() << prop_map;

    PropertyPersistor::toFile<RegisterTestAppl>(*this, QLatin1String("prop.json"));
}


