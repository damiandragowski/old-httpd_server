#include "DDescript.h"

DDescript::DDescript(int size):fd(-1)
{
    m_Buffer = new char[size+1] ;
    assert( m_Buffer != NULL ) ;
    if ( m_Buffer == NULL ) 
        throw DNoMemException() ;
    m_BuffSize = size ; 
}

void DDescript::DeAttach()
{
    try
    {
        Close() ; // close file desc.
    }
    catch ( DInValException err )
    {
        DString( err.Message() ).Printf() ;
        DString( "\nError on close, bad file descriptor\n" ).Printf() ;
        throw DException(this) ;
    }
    fd = -1 ;
}

int DDescript::Fcntl(int cmd) 
{
    int ret = TFR(fcntl(fd, cmd)) ;
    if ( ret == -1 )
        throw DException(this) ;
    return ret ;
}

void DDescript::Attach(int n)
{
    TFR(fcntl(n, F_GETFD)) ; // we check if this is file desc ,this func is not interrupted that says my man
                             // but in POSIX can be so TFR
    if ( n == -1 || errno == EBADF )
        throw DInValException(this) ;
    if ( fd != -1 ) 
    {
        try
        {
            Close() ; // close old file desc.
        }
        catch ( DInValException err )
        {
            DString( err.Message() ).Printf() ;
            DString( "\nError on close, bad file descriptor\n" ).Printf() ;
            throw DException(this) ;
        }
    }
    fd = TFR(dup(n)) ; // attach to object, can't be interrupted
    assert( fd != -1 ) ;
    if ( fd == -1 )
        throw DException(this) ;
}

int DDescript::GetBuffSize() const
{
    return m_BuffSize ;
}

int DDescript::Dup() const
{
    assert( fd != -1 ) ;
    if ( fd == -1 ) 
        throw DInValException(this) ;
    int ret = TFR(dup(fd)) ; // can't be interrupted
    assert( ret != -1 ) ;
    if ( ret == -1 ) 
        throw DException(this) ;
    return ret ;
}

DDescript::DDescript(const DDescript & rhs)
{
    if ( m_Buffer != NULL ) 
        delete [] m_Buffer ;
    if ( fd != -1 ) 
    {
        try
        {
            Close() ; // close old file desc.
        }
        catch ( DInValException err )
        {
            DString( err.Message() ).Printf() ;
            DString( "\nError on close, bad file descriptor\n" ).Printf() ;
            throw DException(this) ;
        }
    }
    try
    {
        fd = rhs.Dup() ;
    } 
    catch( DInValException err )
    {
        DString( err.Message() ).Printf() ;
        DString( "\nDuplication Error\n" ).Printf() ;
        throw DException(this) ;
    }
    catch( DException error )
    {
        DString( error.Message() ).Printf() ;
        DString( "\nDuplication Error\n" ).Printf() ;
        throw DException(this) ;
    }
    m_BuffSize = rhs.m_BuffSize ;
    m_Buffer = new char[m_BuffSize+1] ;
    assert( m_Buffer != NULL ) ;
    if ( m_Buffer == NULL ) 
        throw DNoMemException() ;
    memcpy(m_Buffer, rhs.m_Buffer, m_BuffSize) ;
}

DDescript::~DDescript()
{ 
    assert( m_Buffer != NULL ) ;
    if ( m_Buffer == NULL ) 
        throw DNULLPtrAssigmentException(this) ;
    delete [] m_Buffer ;
    m_Buffer = NULL ;
    if ( fd != -1 ) 
    {
        try
        {
            Close() ; // close old file desc.
        }
        catch ( DInValException err )
        {
            DString( err.Message() ).Printf() ;
            DString( "\nError on close, bad file descriptor\n" ).Printf() ;
            throw DException(this) ;
        }
    }
    m_BuffSize = 0 ;
}                                    
                                    
char * DDescript::getBuffer() const
{ 
    return m_Buffer ; 
}

int DDescript::Read(int n) 
{ 
    if ( n > m_BuffSize )  
        throw DInValException(this) ;
    int len = TFR(read(fd, m_Buffer, n)) ;
    if ( len == -1 )
        throw DSystemException(this) ;
    return len ;
}

int DDescript::ReadLn(int ms, DString & outString, int sret, int & offset)
{
    register int oldret = 0, ret = 0 ;
    register int i = 0 ;
    register char * temp = new char[m_BuffSize+1] ;
    bool cont = true, go = true ;
    int size=m_BuffSize ;

    if ( temp == NULL )
        throw DNoMemException() ;
    // if this is continue of action
    if ( sret != 0 )
    {
        memcpy((void *)(temp), m_Buffer+offset, sret) ;
        go = false ;
        ret = sret ;
    }
    while(cont)
    {
        if ( go )
        {
            try
            {
                ret = SynchRead(ms) ;
            }
            catch ( DSystemException & err )
            {
                DString( err.Message() ).Printf() ;
                DString( "\nError on SynchRead in Readln\n").Printf() ;
                throw DSystemException(this) ;
            }
            if ( ( ret == -1 ) || ( ret == 0 ) )
                return -1 ;
            if ( oldret + ret > m_BuffSize )
            {       
                // reallocate buffer
                char * temp2 ;
                size += size ;
                temp2 = new char[size+1] ;
                if ( temp2 == NULL )
                    throw DNoMemException() ;
                memcpy(temp2, temp, oldret) ;
                delete [] temp ;
                temp = temp2 ;
            }
            memcpy((void *)(temp+oldret), m_Buffer, ret) ;
            offset = 0 ;
        }
        go = true ;
        for ( i = oldret ; i < ret+oldret; i++)
            if( temp[i] == '\n')
            {
                cont = false ;
                break;
            }
        if ( cont )
            oldret += ret ;
    }
    try
    {
        outString.SetBufferAndSize(temp, i+1) ;
    }
    catch( DNULLPtrAssigmentException & err )
    {
        DString( err.Message() ).Printf() ;
        DString( "\nError on Buffer seting in Readln\n").Printf() ;
        throw DNULLPtrAssigmentException(this) ;
    }
    catch( DNoMemException & err )
    {
        DString( err.Message() ).Printf() ;
        DString( "\nError on Buffer seting in Readln, No enough memory\n").Printf() ;
        throw DNoMemException() ;
    }

    ret = ret+oldret - (i+1) ;
    offset += (i+1) - oldret ;
    delete [] temp ;
    return ret ;
}

int DDescript::SynchWrite(int ms,int len)
{
    struct pollfd pollst ;
    int ret ;
    pollst.events = POLLOUT ;
    pollst.fd = fd ;
    ret = TFR(poll(&pollst, 1, ms)) ;
    if ( ret == -1 ) 
        throw DSystemException(this) ;
    if ( ret == 0 )
        return -1 ; // time out
    try
    {
        ret = Write(len) ;
    }
    catch(DSystemException & err)
    {
        DString( err.Message() ).Printf() ;
        DString("\nError on SynchRead Read function\n").Printf() ;
        throw DSystemException(this) ;
    }
    return ret ;
}

int DDescript::SynchRead(int ms)
{
    struct pollfd pollst ;
    int ret ;
    pollst.events = POLLIN ;
    pollst.fd = fd ;
    ret = TFR(poll(&pollst, 1, ms)) ;
    if ( ret == -1 ) 
        throw DSystemException(this) ;
    if ( ret == 0 )
        return -1 ; // time out
    try
    {
        ret = Read() ;
    }
    catch(DSystemException & err)
    {
        DString( err.Message() ).Printf() ;
        DString("\nError on SynchRead Read function\n").Printf() ;
        throw DSystemException(this) ;
    }
    return ret ;
}

int DDescript::Write(int n)
{
    if ( n > m_BuffSize )  
        throw DInValException(this) ;
    int len = TFR(write(fd,m_Buffer,n)) ;
    if ( len == -1 )
        throw DSystemException(this) ;
    return len ;
}

int DDescript::Read(void * Buff, int size)
{
    assert( Buff != NULL & size >= 0 ) ;
    int len = TFR(read(fd, (char *)Buff, size)) ;
    if ( len == -1 )
        throw DSystemException(this) ;
    return len ;
}

int DDescript::Write(const void * Buff, int & size)
{
    assert( Buff != NULL & size >= 0 ) ;
    int len = TFR(write(fd, (char *)Buff, size)) ;
    if ( len == -1 )
        throw DSystemException(this) ;
    return len ;
}

int DDescript::Close()
{ 
    int ret = TFR(close(fd)) ; // My man says that it can't be interrupted, ubt in posix it can be
    if ( ret == -1 )
        throw DInValException(this) ;
    fd = -1 ;
    return ret ;
}

void DDescript::Flush()
{
    fflush(NULL) ;
}
