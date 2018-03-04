#ifndef     DOBJECT_H
#define     DOBJECT_H
#define     DOBJECT_DESCRIPTION "DObject"

#include "headers.h"

class DObject
{
public:
    // return constant poiter for constant buffer
    // method is constant because it doesn't modify object
    void * operator new(size_t size)
    {
        ++m_iCounter ;
        return ::operator new(size) ;
    }
    void * operator new[](size_t size)
    {
        ++m_iCounter ;
        return ::operator new[](size) ;
    }
    void operator delete(void * ptr)
    {
        --m_iCounter ;
        return ::operator delete(ptr) ;
    }
    void operator delete[](void * ptr)
    {
        --m_iCounter ;
        return ::operator delete[](ptr) ;
    }
    static int GetCounter() { return m_iCounter ; }
    virtual const char * const ToString() const { return DOBJECT_DESCRIPTION ; }
private:
    static int m_iCounter ;
} ;

#endif  // DOBJECT_H

