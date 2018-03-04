#ifndef     DHTTPREQUEST_H
#define     DHTTPREQUEST_H
#define     DHTTPREQUEST_DESCRIPTION "DHttpdRequest"

#define     HTTPOK                  200
#define     HTTPFORBIDDEN           403
#define     HTTPNOTFOUND            404
#define     HTTPMETHODNOTALLOWED    405
#define     HTTPINTERNALSERVERERROR 501

#include "headers.h"
#include "DObject.h"
#include "DException.h"
#include "DList.h"
#include "DString.h"
#include "DDescript.h"
#include "DSocket.h"
#include "DFile.h"
#include "DSemaphore.h"
#include "DSharedMem.h"
#include "DFork.h"

class DHttpRequest:public DObject
{
public:
    DHttpRequest(DString & _path) ;
    virtual ~DHttpRequest() ;
    int GetRequestStrings(DSocket & cli, int timeout) ;
    void Printf() const ;
    DString GetMethod() ;
    DString & GetPath() ;
    void DelimPath() ;
    bool IsKeepAlive() ;
    int GetKeepAlive() ;
    DString GetMime() ;
    int GetErrorCode() const { return ErrorCode ; }
    DString & GetFileExtension() { return ext ; }
    virtual const char * const ToString() const { return DHTTPREQUEST_DESCRIPTION ; }
private:
    DAuto_ptr<DPtrList<DString *> > m_pList ;
    DString path ;
    DString ext ;
    int ErrorCode ;
} ;

#endif  // DHTTPREQUEST_H

