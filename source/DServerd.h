#ifndef     DSERVERD_H
#define     DSERVERD_H
#define     DSERVERD_DESCRIPTION         "DServerd"

#include "headers.h"
#include "DObject.h"
#include "DException.h"
#include "DString.h"
#include "DDescript.h"
#include "DSocket.h"
#include "DFile.h"
#include "DHttpRequest.h"

class DServerd:public DObject
{
public:
    DServerd() ;
    virtual ~DServerd() ;
    int Run() ;
    void Help() ;
    int Parsing(int argc, char ** argv) ;
    virtual const char * const ToString() const { return DSERVERD_DESCRIPTION ; }
private:
    DString m_ologFileName ;
    DString m_oDirPath ;
    int     m_iPortNumber ;
    int     m_iAdminPortNumber ;
    int     m_iNumerOfDeamons ;
    DFork   proc ;
} ;

#endif  // DSERVERD_H

