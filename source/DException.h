#ifndef     DEXCEPTION_H
#define     DEXCEPTION_H
#define     DEXCEPTION_DESCRIPTION                          "Exception at "
#define     DSYSTEMEXCEPTION_DESCRIPTION                    "System Exception at "
#define     DNOMEMEXCEPTION_DISCRIPTION                     "No Memory In System"
#define     DOUTOFBOUNDEXCEPTION_DISCRIPTION                "Out of Bound Exception at "
#define     DINVALEXCEPTION_DISCRIPTION                     "Invalidate Value Exception at "
#define     DNULLPOINTERASSIGMENTEXCEPTION_DISCRIPTION      "NULL pointer assigment at "

#include    "DObject.h"

class DException:public DObject
{
public:
    DException():obj(NULL), text(NULL) { }
    DException(const DException & rhs) ;
    DException(const DObject * __this) ;
    DException(const DObject * __this, const char * const sec) ;
    virtual ~DException() ;
    // Only in exceptions ToString method means somethink different
    virtual const char * const ToString() const { return DEXCEPTION_DESCRIPTION ; }
    const char * const Message() const ;
protected:
    DObject * obj ;
    char * text ;
} ;

class DNoMemException:public DException
{
public:
    virtual const char * const ToString() const { return DNOMEMEXCEPTION_DISCRIPTION ; }
} ;

class DOutOfBoundException:public DException
{
public:
    DOutOfBoundException(const DObject * __this)
        :DException(__this, ToString())
    {
    }
    virtual const char * const ToString() const { return DOUTOFBOUNDEXCEPTION_DISCRIPTION ; }
} ;

class DInValException:public DException
{
public:
    DInValException(const DObject * __this)
        :DException(__this, ToString())
    {
    }
    virtual const char * const ToString() const { return DINVALEXCEPTION_DISCRIPTION ; }
} ;

class DSystemException:public DException
{
public:
    DSystemException(const DObject * __this)
        :DException(__this, ToString())
    {
    }
    virtual const char * const ToString() const { return DSYSTEMEXCEPTION_DESCRIPTION ; }
} ;

class DNULLPtrAssigmentException:public DException
{
public:
    DNULLPtrAssigmentException(const DObject * __this)
        :DException(__this, ToString())
    {
    }
    virtual const char * const ToString() const { return DNULLPOINTERASSIGMENTEXCEPTION_DISCRIPTION ; }
} ;

#endif  // DEXCEPTION_H
