#include "DFork.h"

DFork::DFork()
    :m_isChild(true)
{
    DAuto_ptr<DSignal> Temp ( new DSignal() ) ;
    m_SigFunc = Temp ;
    m_pid = getpid() ;
    try
    {
        m_SigFunc->Register(SIGCLD) ;
    }
    catch( DSystemException & err )
    {
        DString("Error in Registering Signal in constructor").Printf() ;
        DString( err.Message() ).Printf() ;
        throw DSystemException(this) ;
    }
    catch( DNoMemException & err )
    {
        DString( err.Message() ).Printf() ;  
        DString("\nError in Ignoring Signal No Memory at DFork Constructor\n").Printf() ;
        throw DNoMemException() ;
    }
}

void DFork::PutSignalObject(DAuto_ptr<DSignal> & obj)
{
    m_SigFunc = obj ;
}

int DFork::RegisterSignal(int signal)  throw( DSystemException, DNoMemException )
{
    int ret ;
    try
    {
        ret = m_SigFunc->Register(signal) ;
    }
    catch( DSystemException & err )
    {
        DString( err.Message() ).Printf() ;
        DString("\nError in Registering Signal\n").Printf() ;
        throw DSystemException(this) ;
    }
    catch( DNoMemException & err )
    {
        DString( err.Message() ).Printf() ;  
        DString("\nError in Registering Signal No Memory\n").Printf() ;
        throw DNoMemException() ;
    }
    return ret ;
}

int DFork::UnRegisterSignal(int signal) throw ( DSystemException )
{
    int ret ;
    try
    {
        ret = m_SigFunc->UnRegister(signal) ;
    }
    catch( DSystemException & err )
    {
        DString( err.Message() ).Printf() ;
        DString("\nError in Registering Signal\n").Printf() ;
        throw DSystemException(this) ;
    }
    return ret ;
}

int DFork::IgnoreSignal(int signal) throw( DSystemException, DNoMemException )
{
    int ret ;
    try
    {
        ret = m_SigFunc->IgnoreSignal(signal) ;
    }
    catch( DSystemException & err )
    {
        DString("Error in Registering Signal").Printf() ;
        DString( err.Message() ).Printf() ;
        throw DSystemException(this) ;
    }
    catch( DNoMemException & err )
    {
        DString( err.Message() ).Printf() ;  
        DString("\nError in Ignoring Signal No Memory\n").Printf() ;
        throw DNoMemException() ;
    }
    return ret ;
}

DFork::~DFork()
{

}

int DFork::SetpGrp() throw ( DSystemException )
{
    int ret ;
    if ( ( ret = TFR( setpgrp() ) ) < 0 )
        throw DSystemException(this) ;
    return ret ;
}

pid_t DFork::MakeDeamon()
{
    if ( TFR(getppid()) == 1 )
        EndInit() ;
    try
    {
#ifdef	SIGTTOU
        IgnoreSignal(SIGTTOU) ;
#endif    
#ifdef	SIGTTIN
        IgnoreSignal(SIGTTIN) ;
#endif
#ifdef  SIGTSTP
        IgnoreSignal(SIGTSTP) ;
#endif
        Fork() ;
        if ( !isChild() ) 
            return 0 ;
        if ( TerminalOut() > 0 )
            EndInit() ;
        else
            return 0 ;
    }
    catch(DNoMemException & err)
    {
        DString( err.Message() ).Printf() ;
        DString( "\nError in starting deamon\n" ).Printf() ;
        throw DNoMemException() ;
    }
    return GetPID() ;
}

void DFork::EndInit()
{
    register int fd ;
    for ( fd = 0; fd < NOFILE; fd++)
        TFR(close(fd)) ;
    errno = 0 ;
    TFR(chdir("/")) ;
    TFR(umask(0)) ;
}

pid_t DFork::TerminalOut()
{
    try
    {
        SetpGrp() ;
        IgnoreSignal(SIGHUP) ;
        Fork() ;
        if ( !isChild() )
            return (pid_t) 0 ;
    }
    catch(DNoMemException & err)
    {
        DString( err.Message() ).Printf() ;
        DString( "\nError at Terminal Out\n" ).Printf() ;
        throw DNoMemException() ;
    }
    catch(DSystemException & err)
    {
        DString( err.Message() ).Printf() ;
        DString( "\nError at Terminal Out\n" ).Printf() ;
        throw DSystemException(this) ;
    }
    return m_pid ;
}



pid_t DFork::Fork() throw ( DSystemException )
{
    pid_t ret ;
    if ( (ret = (pid_t) TFR(fork())) == 0 )
    {
        m_pid = getpid() ;
        m_isChild = true ;
    } 
    else if ( ret > 0 ) 
    {
        m_isChild = false ;
    }
    else
    {
        throw DSystemException(this) ;
    }
    return ret ;
}

bool DFork::isChild()
{
    return m_isChild ;
}

pid_t DFork::GetPID()
{
    return m_pid ;
}


