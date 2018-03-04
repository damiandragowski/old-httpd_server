#include "DSocket.h"

char * DNetAddress::Inet_ntoa()
{
    return inet_ntoa( ((struct sockaddr_in *)this->addr)->sin_addr) ;
}

DNetAddress::DNetAddress(short sin_family, int sin_port, const char * const host)
{
    addr = (sockaddr *) new struct sockaddr_in() ;
    assert( addr != NULL ) ; 
    if ( addr == NULL )
        throw DNoMemException() ;
    addlen = sizeof(struct sockaddr_in) ;
    memset((void *)addr, '\0', addlen) ; //can't be interrupted
    ((struct sockaddr_in *)addr)->sin_family = sin_family ;
    if ( strlen(host) == 0 )
        ((struct sockaddr_in *)addr)->sin_addr.s_addr = htonl(INADDR_ANY) ;
    else
        ((struct sockaddr_in *)addr)->sin_addr.s_addr = inet_addr(host) ;
    ((struct sockaddr_in *)addr)->sin_port = htons(sin_port) ;
}

unsigned int DNetAddress::GetLen() const
{
    return addlen ;
}

DNetAddress::DNetAddress(const DNetAddress & rhs)
{
    if ( addr != NULL )
        delete addr ;
    addr =(sockaddr *) new sockaddr_in() ;
    ZeroBuffer() ;
    memcpy((void *)addr, rhs.addr, sizeof(sockaddr_in)) ;  //can't be interrupted
    addlen = rhs.addlen ;
}

struct sockaddr * DNetAddress::GetAddr() const
{
    assert( addr != NULL ) ;
    if ( addr == NULL )
        throw DNULLPtrAssigmentException(this) ;
    return addr ;
}

void DNetAddress::SetSize(unsigned int n)
{
    addlen = n ;
}

void DNetAddress::ZeroBuffer()
{
    memset(addr, '\0', sizeof(struct sockaddr)) ; //can't be interrupted
    addlen = sizeof(struct sockaddr) ;
}

DNetAddress::~DNetAddress()
{
    assert( addr != 0 ) ;
    if ( addr == NULL ) 
        throw DNULLPtrAssigmentException(this) ;
    delete addr ;
    addlen = 0 ;
}

DSocket::DSocket(int domain, int type, int protocol)
    :address(PF_INET)
{
    try
    {
        Create(domain, type, protocol) ;
    }
    catch( DInValException err )
    {
        DString( err.Message() ).Printf() ;
        DString( "\nCreate Error in consturctor\n" ).Printf() ;
        throw DException(this) ;
    }
}

int DSocket::ShutDown(int mode)
{
    int ret = shutdown(fd, mode) ; // can't be interrupt, why? I don;t know.
    if ( ret == -1 )
        throw DInValException(this) ;
    return ret ;
}

DSocket::~DSocket()
{

}

int DSocket::Listen(int backlog)
{
    int n = TFR(listen(fd, backlog)) ;
    if ( n == -1 )
    {
        DString error ;
        error.Format("\nListen Error number %d\n",errno) ;
        error.Printf() ;
        throw DException(this) ;
    }
    return n ;
}

int DSocket::Create(int domain, int type, int protocol)
{
    fd = TFR(socket(domain, type, protocol)) ;
    assert( fd != -1 ) ;
    if ( fd == -1 )
        throw DInValException(this) ;
    return fd ;
}

int DSocket::Bind(DNetAddress & addr)
{
    address = addr ;
    int ret = TFR(bind(fd, address.GetAddr(), address.GetLen())) ;
    assert( ret != -1 ) ;
    if ( ret == -1 ) 
        throw DException(this) ;
    return ret ;
}



DSocket DSocket::Accept()
{
    DSocket      Client ;
    unsigned int returnval ;
    int          ret ;
    Client.address.ZeroBuffer() ;
    struct sockaddr * temp = Client.address.GetAddr() ;
    ret = TFR(accept(fd, temp, &returnval)) ;
    assert( ret != -1 ) ; 
    if ( ret == -1 ) 
        throw DException(this) ;
    Client.address.SetSize(returnval) ;
    Client.Attach(ret) ;
    return Client ;
}

int DSocket::Connect()
{
    int ret = TFR(connect(fd, address.GetAddr(), address.GetLen())) ;
    assert( ret != -1 ) ; 
    if ( ret == -1 ) 
        throw DException(this) ;
    return ret ;
}

int DSocket::Connect(DNetAddress & rhs)
{
    address = rhs ;
    int ret ;
    try
    {
        ret = Connect() ;
    }
    catch ( DException err )
    {
        DString( err.Message() ).Printf() ;
        DString( "Error on Connect\n" ).Printf() ;
        throw DException(this) ;
    }
    return ret ;
}

DSocket::DSocket(const DSocket & rhs)
{
    address = rhs.address ;
}
