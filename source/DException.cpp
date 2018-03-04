#include "DException.h"


DException::DException(const DObject * __this, const char * const sec)
    :obj(const_cast<DObject *>(__this))
{
    int len = strlen(sec) ;
    assert( obj != NULL ) ;
    len += strlen(obj->ToString()) ;
    text = new char[len+1] ;
    assert( text != NULL ) ;
    text[len] = '\0' ;
    strcpy(text, sec) ;
    strcat(text, obj->ToString()) ;
}

DException::DException(const DException & rhs)
{
    if ( text ) 
        delete [] text ;
    text = new char[strlen(rhs.text)+1] ;
    assert( text != NULL ) ;
    strcpy(text, rhs.text) ;
    obj = rhs.obj ;
    assert( obj != NULL ) ;
}


DException::DException(const DObject * __this)
    :obj(const_cast<DObject *>(__this))
{
    int len = strlen(ToString()) ;
    assert( obj != NULL ) ;
    len += strlen(obj->ToString()) ;
    text = new char[len+1] ;
    assert( text != NULL ) ;
    text[len] = '\0' ;
    strcpy(text, ToString()) ;
    assert( obj != NULL ) ;
    strcat(text, obj->ToString()) ;
}


DException::~DException()
{
    if ( text )
        delete [] text ;
    text = NULL ;
}

const char * const DException::Message() const
{
    return (text) ? text : "" ;
}

