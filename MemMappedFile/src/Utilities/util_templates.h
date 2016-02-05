#ifndef ITEMPLATES_HPP
#define ITEMPLATES_HPP

#include <QVariant>

// ----------------------------------------------------------------------------
//
// Template ISingleton
//
// ----------------------------------------------------------------------------
template<class T>
class ISingleton
{
public:
    static T& getInstance()
    {
        static T m_instance;
        return m_instance;
    }
private:
        ISingleton();                                 // ctor hidden
        ISingleton(ISingleton const&);                // copy ctor hidden
        ISingleton& operator=(ISingleton const&);     // assign op. hidden
        ~ISingleton();                                // dtor hidden
};


// ----------------------------------------------------------------------------
//
// Template CallbackBaseClass
//
// ----------------------------------------------------------------------------
template < class Class, typename ReturnType, typename Parameter >
class CallbackBaseClass
{
public:
    // typedef Method
    typedef ReturnType (Class::*Method)(Parameter);

    CallbackBaseClass(Class* _class_instance, Method _method) : m_p_return_value(0), m_p_parameter(0)
    {
        m_p_class_instance	= _class_instance;
        m_method			= _method;
    }

    void SetParameter(Parameter* in_parameter)
    {
        m_p_parameter = in_parameter;
    }

    void SetReturnPtr(ReturnType* in_return)
    {
        m_p_return_value = in_return;
    }

    void operator()()
    {
        if (m_p_parameter == 0)
            m_p_return_value = (m_p_class_instance->*m_method)();
        else
            m_p_return_value = (m_p_class_instance->*m_method)(&m_p_parameter);
    }

    ReturnType operator()(Parameter parameter)
    {
        return (m_p_class_instance->*m_method)(parameter);
    }

    ReturnType execute(Parameter parameter)
    {
        return operator()(parameter);
    }

private:
    Class* m_p_class_instance;
    Method  m_method;
    ReturnType* m_p_return_value;
    Parameter* m_p_parameter;
};


// ----------------------------------------------------------------------------
//
// Template VPtr
//
// ----------------------------------------------------------------------------
template <class T> class VPtr
{
public:
    static T* toPtr(QVariant v)
    { return  (T *) v.value<void *>(); }

    static QVariant toQVariant(T* ptr)
    { return qVariantFromValue((void *) ptr); }
};



///////////////////////////////////////////////////////////////////////
// GenericFactory - definition & implementation of
// generic abstract factory class
//
// Design: Jim Hyslop, see article http://www.ddj.com/cpp/184403786
// Coding: Joerg Schlager / schlagej / joerg.schlager@avl.com
///////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//----------------------------------------------------------------------
#include <map>
//----------------------------------------------------------------------
// singleton generic abstract factory class
template <class BaseType, typename ClassIDKey=std::string>
class GenericFactory
{
    // change return type of create function to base type ptr
    typedef BaseType* (*CreateMethod)();
    typedef std::map<ClassIDKey, CreateMethod> CreateMethodRegistry;

private:
    // singleton, private constructor, no copy constructor
    GenericFactory() {}
    GenericFactory(const GenericFactory&) { ASSERT(FALSE); }
    GenericFactory &operator = (const GenericFactory&) { ASSERT(FALSE); }

    CreateMethodRegistry m_registry;

public:
    BaseType* Create(const ClassIDKey &key) const;
    void RegisterCreateMethod(const ClassIDKey &key, CreateMethod method);

    // retrieve the singleton instance
    static GenericFactory& GetInstance()
    {
        static GenericFactory<BaseType,ClassIDKey> factory;
        return factory;
    }
};
//----------------------------------------------------------------------
template <class BaseType, typename ClassIDKey>
BaseType* GenericFactory<BaseType,ClassIDKey>::Create(const ClassIDKey &key) const
{
    CreateMethodRegistry::const_iterator reg_entry = m_registry.find(key);
    if( reg_entry != m_registry.end())
        return(reg_entry->second());

    return(NULL);
}
//----------------------------------------------------------------------
template <class BaseType, typename ClassIDKey>
void GenericFactory<BaseType,ClassIDKey>::RegisterCreateMethod(const ClassIDKey &key, CreateMethod method)
{
    m_registry[key] = method;
}
//----------------------------------------------------------------------
// template class to encapsulate the registration process
// base type: abstract factory return type
// manufactured type: concrete object type,
// class id: id of concrete object
template <class BaseType, class ManufacturedType, typename ClassIDKey=std::string>
class RegisterInFactory
{
public:
    static BaseType* CreateInstance()
    {
        return((BaseType*)(new ManufacturedType));
    }

    // constructor performs registration in generic factory!
    RegisterInFactory(const ClassIDKey & id)
    {
        GenericFactory<BaseType>::GetInstance().RegisterCreateMethod(id,CreateInstance);
    }

};
#endif // ITEMPLATES_HPP
