#ifndef     DSIGNAL_H
#define     DSIGNAL_H
#define     DSIGNAL_DESCRIPTION         "DSignal"

#include "DObject.h"
#include "DException.h"
#include "DString.h"
#include "DList.h"
#include "headers.h"

class DSignal:public DObject
{
public:
    DSignal() ;
    virtual ~DSignal() throw ( DNULLPtrAssigmentException, DSystemException ) ;
    int Register(int signal) throw( DSystemException, DNoMemException ) ;
    void UnRegisterAll() throw ( DNULLPtrAssigmentException, DSystemException ) ;
    int UnRegister(int signal) throw( DSystemException ) ;
    int IgnoreSignal(int signal) throw( DSystemException, DNoMemException ) ;
    static void UseSignal(int signal) throw ( DNULLPtrAssigmentException ) ;
    virtual void SigALARM(int signal) { }
    virtual void SigCHILD(int signal) ;
    virtual void SigCONT(int signal) { }
    virtual void SigHUP(int signal) { }
    virtual void SigINT(int signal) { }
    virtual void SigPIPE(int signal) { }
    virtual void SigQUIT(int signal) { }
    virtual void SigSTOP(int signal) { }
    virtual void SigTERM(int signal) { }
    virtual void SigTRAP(int signal) { }
    virtual void SigTSTP(int signal) { }
    virtual void SigTTIN(int signal) { }
    virtual void SigTTOU(int signal) { }
    virtual void SigURG(int signal) { }
    virtual void SigUSR1(int signal) { }
    virtual void SigUSR2(int signal) { }
    virtual void SigXCPU(int signal) { }
    virtual const char * const ToString() const { return DSIGNAL_DESCRIPTION ; }
protected:
    static DSignal * __this ;
    DAuto_ptr<DList<int> > m_aList ;
} ;

#endif  // DSIGNAL_H

