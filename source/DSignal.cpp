#include "DSignal.h"

DSignal * DSignal::__this = NULL ;
    
DSignal::DSignal()
{
    DAuto_ptr<DList<int> > Temp ( new DList<int> ) ;
    m_aList = Temp ;
    __this = this ;
}

DSignal::~DSignal()  throw ( DNULLPtrAssigmentException, DSystemException )
{
    try
    {
        UnRegisterAll() ;
    }
    catch( DNULLPtrAssigmentException & err )
    {
        DString( err.Message() ).Printf() ;
        DString("\nError on Destructor\n").Printf() ;
        throw DNULLPtrAssigmentException(this) ;
    }
    catch( DSystemException & err )
    {
        DString( err.Message() ).Printf() ;
        DString("\nError on Destructor\n").Printf() ;
        throw DNULLPtrAssigmentException(this) ;
    }
}

void DSignal::UnRegisterAll() throw ( DNULLPtrAssigmentException, DSystemException )
{
    register int i, signal, size = m_aList->GetSize() ;
    for (i = 0; i < size; i++)
    {
        try
        {
            signal = m_aList->RemoveHead() ;
            UnRegister(signal) ;
        }
        catch ( DNULLPtrAssigmentException & err )
        {
            DString( err.Message() ).Printf() ;
            DString("\nError on Removing Head for list in UnRegisterAll Method\n").Printf() ;
            throw DNULLPtrAssigmentException(this) ;
        }
        catch ( DSystemException & err )
        {
            DString( err.Message() ).Printf() ;
            DString("\nError on Unregistering signal  in UnRegisterAll Method\n").Printf() ;
            throw DSystemException(this) ;
        }
    }
}

void DSignal::UseSignal(int signal) throw ( DNULLPtrAssigmentException )
{
    assert( __this != NULL ) ;
    if ( __this == NULL )
        throw DNULLPtrAssigmentException(__this) ;
    switch (signal) 
    {
        case SIGALRM:
            __this->SigALARM(signal) ;
            break;
        case SIGCLD:
            __this->SigCHILD(signal) ;
            break;
        case SIGCONT:
            __this->SigCONT(signal) ;
            break;
        case SIGHUP:
            __this->SigHUP(signal) ;
            break;
        case SIGINT:
            __this->SigINT(signal) ;
            break;
        case SIGPIPE:
            __this->SigPIPE(signal) ;
            break;
        case SIGQUIT:
            __this->SigQUIT(signal) ;
            break;
        case SIGSTOP:
            __this->SigSTOP(signal) ;
            break;
        case SIGTERM:
            __this->SigTERM(signal) ;
            break;
        case SIGTRAP:
            __this->SigTRAP(signal) ;
            break;
        case SIGTSTP:
            __this->SigTSTP(signal) ;
            break;
        case SIGTTIN:
            __this->SigTTIN(signal) ;
            break;
        case SIGTTOU:
            __this->SigTTOU(signal) ;
            break;
        case SIGURG:
            __this->SigURG(signal) ;
            break;
        case SIGUSR1:
            __this->SigUSR1(signal) ;
            break;
        case SIGUSR2:
            __this->SigUSR2(signal) ;
            break;
        case SIGXCPU:
            __this->SigXCPU(signal) ;
            break;
    } ;
}

// void SetSignal

int DSignal::IgnoreSignal(int signal) throw( DSystemException, DNoMemException )
{
    int ret ;
    struct sigaction sa ;
    sa.sa_flags = 0 ;
    sa.sa_handler = SIG_IGN ;
    sigemptyset(&sa.sa_mask) ;
    sigaddset(&sa.sa_mask, signal) ;    
    if ( ( ret = TFR(sigaction(signal, &sa, NULL)) ) < 0 )
        throw DSystemException(this) ;
    try
    {
        m_aList->AddHead(signal) ;
    }
    catch( DNoMemException & err )
    {
        printf(err.Message()) ;
        printf("\nError in Ingnoring signal\n") ;
        throw DNoMemException() ;
    }
    return ret ;
}

int DSignal::UnRegister(int signal) throw( DSystemException )
{
    int ret ;
    struct sigaction sa ;
    sa.sa_flags = 0 ;
    sa.sa_handler = SIG_DFL ;
    sigemptyset(&sa.sa_mask) ;
    sigaddset(&sa.sa_mask, signal) ;    
    if ( ( ret = TFR(sigaction(signal, &sa, NULL)) ) < 0 )
        throw DSystemException(this) ;
    return ret ;
}

int DSignal::Register(int signal) throw( DSystemException, DNoMemException )
{
    int ret ;
    struct sigaction sa ;
    sa.sa_flags = 0 ;
    switch (signal)
    {
        case SIGCLD:
            sa.sa_flags = SA_NOCLDSTOP ;
            sa.sa_handler = DSignal::UseSignal ;
            break;
        default:
            sa.sa_handler = DSignal::UseSignal ;
            break;
    } ;
    sigemptyset(&sa.sa_mask) ;
    sigaddset(&sa.sa_mask, signal) ;    
    if ( ( ret = TFR(sigaction(signal, &sa, NULL)) ) < 0 )
        throw DSystemException(this) ;
    try
    {
        m_aList->AddHead(signal) ;
    }
    catch( DNoMemException & err )
    {
        printf(err.Message()) ;
        printf("\nError in registering signal\n") ;
        throw DNoMemException() ;
    }

    return ret ;
}

void DSignal::SigCHILD(int signal)
{
    int olderrno = errno ;
    int pid ;
    union wait status ;
    while ( (pid = wait3(&status, WNOHANG, (struct rusage *) 0)) > 0 )
        ; //nop
    errno = olderrno ;
}


