#ifndef     DSOCKET_H
#define     DSOCKET_H
#define     DSOCKET_DESCRIPTION         "DSocket"
#define     DNETADDRESS_DESCRIPTION     "DNetAddress"

#include "DDescript.h"
#include "headers.h"

class DNetAddress:public DObject
{
public:
    DNetAddress(short sin_family = PF_INET, int sin_port = 8080, const char * const host = "255.255.255.255") ;
    DNetAddress(const DNetAddress & rhs) ;
    struct sockaddr * GetAddr(void) const ;
    unsigned int GetLen(void) const ;
    void ZeroBuffer() ;
    void SetSize(unsigned int n) ;
    virtual ~DNetAddress() ;
    char * Inet_ntoa() ;
    virtual const char * const ToString() const { return DNETADDRESS_DESCRIPTION ; }
private:
    struct sockaddr * addr ;
    unsigned int      addlen ;
} ;

class DSocket:public DDescript
{
public:
    DSocket(int domain = PF_INET, int type = SOCK_STREAM, int protocol = 0) ;
    DSocket(const DSocket & rhs) ;
    virtual ~DSocket() ;
    int Create(int domain = PF_INET, int type = SOCK_STREAM, int protocol = 0) ;
    int Connect() ;
    int Connect(DNetAddress &rhs) ;
    int ShutDown(int mode = SHUT_RDWR) ;
    DSocket Accept() ;
    int Listen(int backlog = 5) ;
    int Bind(DNetAddress & addr) ;
    DNetAddress & GetAddres() { return address ; }
    virtual const char * const ToString() const { return DSOCKET_DESCRIPTION ; }
protected:
    DNetAddress address ;
} ;

#endif  // DSOCKET_H

