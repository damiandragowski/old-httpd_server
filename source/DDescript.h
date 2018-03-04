#ifndef     DDESCRIPT_H
#define     DDESCRIPT_H
#define     DDESCRIPT_DESCRIPTION "DDescript"
#define     BUFFERSIZE 4096
#define     MAXSAFEBUFFSIZE 262144

#include "DObject.h"
#include "DException.h"
#include "DString.h"

class DDescript:public DObject
{
public:
    DDescript(int size = BUFFERSIZE) ;
    DDescript(const DDescript & rhs) ;
    virtual ~DDescript() ;
    void Attach(int n) ;
    void DeAttach() ;
    int Dup() const ;
    int Close() ;
    int Read(int n = BUFFERSIZE) ;
    int Read(void * Buff, int size) ;
    int SynchRead(int ms) ;
    int SynchWrite(int ms,int len) ;
    int ReadLn(int ms, DString & outString, int sret, int & offset) ;
    int Write(const void * Buff, int & size) ;
    int Write(int n = BUFFERSIZE) ;
    int Fcntl(int cmd) ;
    void Flush() ;
    int GetBuffSize() const ;
    char * getBuffer() const ;
    int GetDescriptor() const { return fd ; }
    virtual const char * const ToString() const { return DDESCRIPT_DESCRIPTION ; }
protected:
    int  fd ;
    int  m_BuffSize ;
    char * m_Buffer ;
} ;

#endif  // DDESCRIPT_H

