#ifndef     DFORK_H
#define     DFORK_H
#define     DFORK_DESCRIPTION         "DFork"

#include "DObject.h"
#include "DSignal.h"
#include "DString.h"
#include "DList.h"
#include "headers.h"

class DFork:public DObject
{
public:
    DFork() ;
    virtual ~DFork() ;
    pid_t Fork() throw ( DSystemException ) ;
    pid_t MakeDeamon() ;
    void EndInit() ;
    pid_t TerminalOut() ;
    bool isChild() ;
    int SetpGrp() throw ( DSystemException ) ;
    pid_t GetPID() ;
    int RegisterSignal(int signal)  throw( DSystemException, DNoMemException ) ;
    int UnRegisterSignal(int signal) throw ( DSystemException ) ;
    int IgnoreSignal(int signal) throw( DSystemException, DNoMemException ) ;
    void PutSignalObject(DAuto_ptr<DSignal> & obj) ;
    virtual const char * const ToString() const { return DFORK_DESCRIPTION ; }
protected:
    pid_t   m_pid ;
    bool    m_isChild ;
    DAuto_ptr<DSignal> m_SigFunc ;
} ;

#endif  // DFORK_H

