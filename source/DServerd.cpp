#include "DServerd.h"

#define DSIGNAL2_DESCRIPTION "DSignal2"

struct MSG
{
    char text[256] ;
} ;
    
struct Logs
{
    int Counter ;
    MSG Message[20] ;
} ;

class DSignal2:public DSignal
{
public:
    virtual const char * const ToString() const { return DSIGNAL2_DESCRIPTION ; }
    void SigUSR1(int signal) ;
    void SigPIPE(int signal) ;
} ;

void DSignal2::SigPIPE(int signal)
{
    
}

void DSignal2::SigUSR1(int signal)
{
    try
    {
        DSharedMem<int> ProcessID ;
        int * pid ;
        try
        {
            ProcessID.Create(555666L) ;
            pid = ProcessID.GetPermToBuffer() ;
        }
        catch ( DException & err )
        {
            ProcessID.Open(555666L) ;
            pid = ProcessID.GetPermToBuffer() ;
        }

        if ( getpid() != *pid ) 
        {
            DSemaphore lock ;
            lock.Open(443233) ;
            lock.P() ;
            lock.V() ;
        }
    }
    catch ( DException & err )
    {
        printf(err.Message()) ;
    }
}

DServerd::DServerd()
{
    m_iAdminPortNumber = htons(8081) ;
    m_iPortNumber = htons(8080) ;
    m_iNumerOfDeamons = 4 ;
    m_oDirPath = "/" ;
    m_ologFileName = "httpd.log" ;
}

DServerd::~DServerd()
{

}

int DServerd::Parsing(int argc, char ** argv)
{
    int c ;
    int ret = 0 ;
    char * end ;

    static struct option long_options[]=
    {
        {"port", required_argument, 0, 'p'},
        {"admin", required_argument, 0, 'a'},
        {"number",  required_argument, 0, 'n'},
        {"directory",  required_argument, 0, 'd'},
		{"logfile",  required_argument, 0, 'l'},
        {"help"  ,  no_argument, 0, 'h'},
        {0, 0, 0, 0}
    } ;
    assert( argv != NULL ) ;
    while (1)
    {
        c = getopt_long(argc, argv, "p:a:n:d:l:h", long_options, &ret) ;
        if ( c == -1 ) 
            break ;
        switch( c )
        {
            case 'p':
                m_iPortNumber = static_cast<int>(strtol(optarg, &end, 10)) ;
                if ( errno == ERANGE || errno == EINVAL ) 
                {
                    try
                    {
                        DString("Wrong port").Printf() ;
                    }
                    catch(DNoMemException err)
                    {
                        printf(err.Message()) ;
                    }
                    return -1 ;
                }
                break;
            case 'a':
                m_iAdminPortNumber = static_cast<int>(strtol(optarg, &end, 10)) ;
                if ( errno == ERANGE || errno == EINVAL ) 
                {
                    try
                    {
                        DString("Wrong admin").Printf() ;
                    }
                    catch(DNoMemException err)
                    {
                        printf(err.Message()) ;
                    }
                    throw DException(this) ;
                }
                break;
            case 'n':
                m_iNumerOfDeamons = static_cast<int>(strtol(optarg, &end, 10)) ;
                if ( errno == ERANGE || errno == EINVAL ) 
                {
                    try
                    {
                        DString("Wrong number").Printf() ;
                    }
                    catch(DNoMemException err)
                    {
                        printf(err.Message()) ;
                    }
                    throw DException(this) ;
                }
                break;
            case 'd':
                try
                {
                    m_oDirPath = DString(optarg) ;
                }
                catch(DNULLPtrAssigmentException err)
                {
                    DString( err.Message() ).Printf() ;
                    throw DException(this) ;
                }
                catch(DNoMemException err)
                {
                    printf(err.Message()) ;
                    throw DException(this) ;
                }
                break;
            case 'l':
                try
                {
                    m_ologFileName = DString(optarg) ;
                }
                catch(DNULLPtrAssigmentException err)
                {
                    DString( err.Message() ).Printf() ;
                    throw DException(this) ;
                }
                catch(DNoMemException err)
                {
                    printf(err.Message()) ;
                    throw DException(this) ;
                }

				break;
            case 'h':
            default:
                Help() ;
                return -1 ;
        }
    }
    if( optind < argc || argc < 2 )
    {
         Help() ;
         return -1 ;
    }
    return 0 ;
}

int DServerd::Run()
{
    // Here is server object
    try
    {
        // initliazie sockets

        DAuto_ptr<DSignal> signn ( new DSignal2() ) ;
        proc.PutSignalObject(signn) ;
        proc.IgnoreSignal(SIGPIPE) ;
        proc.IgnoreSignal(SIGINT) ;
        proc.RegisterSignal(SIGUSR1) ;

        if ( proc.MakeDeamon() == 0 )
            return 0 ;

        DSemaphore sem ;
        DSemaphore semtosh ;
        DSharedMem<int> ProcessID ;
        DSharedMem<Logs> logs ;
        DSocket sock1 ;
        DSocket sock2 ;
        DNetAddress addr1(PF_INET, m_iPortNumber, "") ;
        DNetAddress addr2(PF_INET, m_iAdminPortNumber, "") ;
        int timeout = 3*60*1000 ;      // 3 min
        int len, ret, ErrorCode = 200 ;
        bool Yes = true ;

        sock1.Bind(addr1) ;
        sock1.Listen() ;
        sock2.Bind(addr2) ;
        sock2.Listen() ;
        sem.Create(4432) ;
        semtosh.Create(777111L) ;
        ProcessID.Create(555666L) ;
        logs.Create(443322L) ;
        int * pid = ProcessID.GetPermToBuffer() ;
        Logs * ptr = logs.GetPermToBuffer() ;
        *(ProcessID.GetBuffer()) = 0 ;
        DFile log(m_ologFileName) ;
        try
        {
            log.Open(O_CREAT|O_APPEND|O_WRONLY) ;
        }
        catch ( DException & err )
        {
            log.Open(O_CREAT) ;
            log.Close() ;
            log.Open(O_APPEND|O_WRONLY) ;
        }

        for (int i = 0; i < m_iNumerOfDeamons-1; i++)
        {
            proc.Fork() ;
            if ( proc.isChild() )
                break ;
        }

        fd_set readfd ;

        while(1)
        {
            Yes = true ;
            FD_ZERO(&readfd) ;
            FD_SET(sock1.GetDescriptor(), &readfd) ;
            FD_SET(sock2.GetDescriptor(), &readfd) ;

            sem.P() ;
            if ( TFR(select(Max(sock1.GetDescriptor(), sock2.GetDescriptor()) + 1, &readfd, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0)) < 0 )
                throw DException(this) ;
            
            if ( FD_ISSET(sock1.GetDescriptor(), &readfd) ) 
            {
                try
                {
                    DSocket cli = sock1.Accept() ;
                    sem.V() ;
                    while ( Yes ) 
                    {
                        ErrorCode = 200 ;
                        timeout = 3*60*1000 ;
                        DHttpRequest httpreq(m_oDirPath) ;
                        DString Header, ConnState("close") ;

                        if ( httpreq.GetRequestStrings(cli, timeout) == -1 ) 
                        {
                            cli.ShutDown() ;
                            cli.Close() ;
                            break ;
                        }
                    
                        if ( httpreq.IsKeepAlive() )
                        {
                            if ( ( ret = httpreq.GetKeepAlive()) != -1 )
                            {
                                timeout = ret * 1000 ;
                                Yes = true ;
                                ConnState = " keep-alive" ;
                            }
                        }
                        else
                        {
                            Yes = false ;
                        }
                
                        if ( httpreq.GetMethod() != "GET" )
                        {
                            ErrorCode = 405 ;
                        }



                        if ( ErrorCode == 200 )
                        {
                            httpreq.DelimPath() ;
                        }

                        // priority of && operator :)
                        if ( ( ErrorCode == 200 ) && ( ( ret = httpreq.GetErrorCode() ) != 200 ) )
                        {
                            ErrorCode = ret ;
                        }
                        DString filename ;
                        DString mime ;

                        // if we have error

                        if ( ErrorCode != 200 )
                        {
                            filename.Format("%s%d.html", m_oDirPath.GetBuffer(), ErrorCode) ;
                            mime = "text/html" ;
                            Yes = false ;
                            ConnState = "close" ;
                        }
                        else
                        {
                            filename = httpreq.GetPath() ;
                            mime = httpreq.GetMime() ;
                        }

                    
                        DString Logi("Access from : ") ;
                        DString pist ;

                        Logi += DString(cli.GetAddres().Inet_ntoa()) + " Getting filename " + filename ;
                    
                        DFile file(filename) ;

                        file.Open() ;
                        len = file.GetFileLength() ;
                        pist.Format(" File Length = %d ErrorCode = %d\n", len, ErrorCode) ;
                        Logi += pist ;

                        semtosh.P() ;
                        if ( ptr->Counter == 19 )
                        {
                            // saveing
                            for ( int i = 0; i < 20; i++) 
                            {
                                int lens = strlen((ptr->Message[i].text)) ;
                                log.Write((void *)(ptr->Message[i].text), lens) ;
                            }
                            ptr->Counter = 0 ;
                        }
                        ptr->Message[ptr->Counter].text[256] = '\0' ;
                        memcpy((ptr->Message[ptr->Counter].text), Logi.GetBuffer(), 255) ;
                        ++ptr->Counter ;

                        semtosh.V() ;

                        Header.Format("HTTP/1.1 %d\r\nConnection: %s\r\nContent-type: %s\r\nContent-length: %d\r\n\r\n", ErrorCode, ConnState.GetBuffer(), mime.GetBuffer(), len) ;
                
                        len = Header.GetLength() ;
                        int tmp1 = 0 ;
                        int size1 = len ;
                        while ( len != 0  )
                        {
                            size1 = len ;
                            tmp1 = cli.Write((const void *) (Header.GetBuffer() + tmp1) , size1 ) ;
                            len -= tmp1 ;
                        }

                        while( (len = file.Read()) != 0 )
                        {
                            tmp1 = 0 ;
                            int size = len ;
                            while ( size != 0 )
                            {
                                size1 = size ;
                                tmp1 = cli.Write((const void *) (file.getBuffer()+tmp1), size1 ) ;
                                size -= tmp1 ;
                            }
                        }


                        file.Close() ;

                        if ( Yes == false )
                        {
                            cli.ShutDown() ;
                            cli.Close() ;
                        }
                    }
                }
                catch ( DException & err )
                {

                }
            }
            if ( FD_ISSET(sock2.GetDescriptor(), &readfd) ) 
            {
                try
                {
                    DSocket cli = sock2.Accept() ;
                    sem.V() ;
                    bool processing = true ;
                    while (processing)
                    {
                        timeout = 3*60*1000 ;
                        int ret = 0, offset = 0 ;
                        DString str("") ;
                        DString suspend("suspend\r\n") ;
                        DString resume("resume\r\n") ;
                        DString stop("quit\r\n") ;
                        DString quit("stop\r\n") ;

                        bool x = true ;
                        while ( (ret != -1) && x ) 
                        {
                            ret = cli.ReadLn(timeout, str, ret, offset) ;
                            if ( ( str == suspend ) || ( str == resume )
                                    || ( str == quit ) || ( str == stop ) )
                            {
                                x = false ;
                            }
                            if ( x )
                            {
                                try
                                    {
                                    int len ;
                                    DString Header("Bad command !!!\r\n")  ;
                                    len = Header.GetLength() ;
                                    int tmp1 = 0 ;
                                    int size1 = len ;
                                    while ( len != 0  )
                                    {
                                        size1 = len ;
                                        tmp1 = cli.Write((const void *) (Header.GetBuffer() + tmp1) , size1 ) ;
                                        len -= tmp1 ;
                                    }
                                }
                                catch ( DException & err )
                                {

                                }
                            }
                        }
                        if ( ret == -1 )
                        {
                            cli.ShutDown() ;
                            cli.Close() ;
                        }
                        else
                        {
                            if ( str == quit )
                            {
                                cli.ShutDown() ;
                                cli.Close() ;
                                log.Close() ;
                                TFR(kill(-TFR(getpgrp()), SIGQUIT)) ;
                            }
                            else if ( str == suspend ) 
                            {
                                if ( *pid != proc.GetPID() )
                                {
                                    *pid = (int)proc.GetPID() ;
                                    DSemaphore lock ;
                                    lock.Create(443233) ;
                                    lock.P() ;
                                    TFR(kill(-TFR(getpgrp()), SIGUSR1)) ;
                                    sleep(3) ;
                                }
                            }
                            else if ( str == resume ) 
                            {
                                *pid = 0 ;
                                DSemaphore lock ;
                                lock.Open(443233) ;
                                lock.V() ;
                            }
                            else if ( str == stop )
                            {
                                semtosh.P() ;
                                for ( int i = 0; i < ptr->Counter; i++) 
                                {
                                    int lens = strlen((ptr->Message[i].text)) ;
                                    log.Write((void *)(ptr->Message[i].text), lens) ;
                                }
                                ptr->Counter = 0 ;
                                semtosh.V() ;
                                cli.ShutDown() ;
                                cli.Close() ;
                                processing = false ;
                            }
                        }
                    }
                }
                catch( DException & err )
                {
                        
                }
            }
        }
    }
    catch ( DException & err )
    {
        printf( err.Message() ) ;
    }
    return 0 ;
}

void DServerd::Help()
{
    DString("This is http server. Written by Damian Dragowski\n\n").Printf() ;
    DString("usage: httpd --port(p) --admin(a) --number(n) --directory(d) --logfile(l)\n\n").Printf() ;
    DString("\tport      - port on which server will listen for clients.\n").Printf() ;
    DString("\tadmin     - port on which server will listen for admins.\n").Printf() ;
    DString("\tnumber    - How many servers will be working.\n").Printf() ;
    DString("\tdirectory - ( must be end with / ) What is the startup directory for files.\n").Printf() ;
    DString("\tlogfile   - Name of log file.\n\n").Printf() ;
    DString("Example: ./httpd -p 80 -a 8080 -n 4 -d /www/ -l httpd.log\n").Printf() ;
    DString("\n\nWarnning all files with erros number ex.(404.html) must be in directory !!!\n").Printf() ;
    DString("Warnning after stopping server You must to wait for system unbind\n").Printf() ;
}
