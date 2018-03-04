#ifndef		DSTRING_H
#define		DSTRING_H
#define     DSTRING_CLASSNAME "DString"

#include "DList.h"
#include "DException.h"


class DString:public DObject
{
public:
    DString(const char * const Value = "") ;
    DString(int size) ;
    DString(const DString & rhs) ;
    DString(const void * buff, int size) ;
    int GetLength(void) const { return m_Length ; }
    char & operator []( int nIndex ) ;
    void SetAt(char Value, int nIndex) ;
    bool IsEmpty(void) { return m_Length == 0 ; }
    int GetSize() const { return m_Length ; }
    void MakeUpper(void) ;
    DString ToUpper(void) const ;
    DString & Format(const char * text, ...) ;
    int Printf() { return printf( this->GetBuffer() ) ; }
    virtual ~DString() ;
    virtual DString& operator=(const DString & rhs) ;
    virtual DString& operator+=(const DString & rhs) ;
    virtual DString operator+(const DString & rhs) const ;
    const char * GetBuffer(void) const { return (m_Buffer)?m_Buffer:"" ; }
    operator const char * () { return m_Buffer ; }
    bool operator==(const DString & rhs) ;
    bool operator==(const char * const buff) ;
    bool operator!=(const DString & rhs) ;
    bool operator!=(const char * const buff) ;
    bool operator>(const DString & rhs) ;
    bool operator<(const DString & rhs) ;
    bool operator>=(const DString & rhs) ;
    bool operator<=(const DString & rhs) ;
    virtual const char * const ToString() const { return DSTRING_CLASSNAME ; }
    void SetBufferAndSize(char * buff, int size) ;
    void Tokenizer(DString & delim, DPtrList<DString *> & array) ;
    DString SubString(int start, int end) ;
    void DecodeUrl() ;
    char DecodeChar(char * what) ;
    void Deleminate(DString & what) ;
protected:
    int m_Length ;
private:
    char * m_Buffer ;
} ;

typedef DString * DStringPtr ;

#endif  // DSTRING_H
