#include "DString.h"

DString::DString(const char * const Value)
{
    m_Length = strlen(Value) ;
    assert( m_Length >= 0 ) ;
    if ( m_Length < 0 )
        throw DInValException(this) ;
    m_Buffer = new char[m_Length+1] ;
    assert( m_Buffer != NULL ) ;
	if ( m_Buffer == NULL )
		throw DNoMemException() ;
    m_Buffer[m_Length] = '\0' ;
    memcpy(static_cast<void *>(m_Buffer), static_cast<const void *>(Value), m_Length * sizeof(char)) ;
}

DString::~DString()
{
    if ( m_Buffer != NULL )
    {
        delete [] m_Buffer ;
        m_Buffer = NULL ;
        m_Length = 0 ;
    }
}

DString::DString(const void * buff, int size)
{
    m_Length = size ;
    if ( m_Length < 0 )
        throw DInValException(this) ;
    m_Buffer = new char[m_Length+1] ;
    assert( m_Buffer != NULL ) ;
	if ( m_Buffer == NULL )
		throw DNoMemException() ;
    m_Buffer[m_Length] = '\0' ;
    memcpy(static_cast<void *>(m_Buffer), buff, m_Length) ;
}

DString& DString::operator=(const DString & rhs)
{
    assert( &rhs > NULL ) ;
    if ( &rhs == NULL ) 
        throw DNULLPtrAssigmentException(this) ;
    if ( &rhs == this ) 
        return *this ;
    if ( m_Buffer != NULL )
        delete [] m_Buffer ;
    m_Buffer = new char[rhs.m_Length+1] ;
    assert( m_Buffer != NULL ) ;
    if ( m_Buffer == NULL )
        throw DNoMemException() ;
    m_Length = rhs.m_Length ;
    m_Buffer[m_Length] = '\0' ;
    memcpy(static_cast<void *>(m_Buffer), static_cast<const void *>(rhs.m_Buffer), m_Length * sizeof(char)) ;
    return *this ;
}

DString DString::SubString(int start, int end)
{
    if ( start < 0 || end > m_Length )
        throw DOutOfBoundException(this) ;
    int size = end - start ;
    try
    {
        DString ret(m_Buffer+start, size) ;
        return ret ;
    }
    catch (DNoMemException & err)
    {
        printf(err.Message()) ;
        printf("\nError not enoght memory to initiazling SubString\n") ;
        throw DNoMemException() ;
    }
    catch (DInValException & err)
    {
        printf(err.Message()) ;
        printf("\nInvalidate paramter in constructor at SubString\n") ;
        throw DInValException(this) ;
    }
}

DString::DString(const DString & rhs)
{
    m_Length = rhs.m_Length ;
    assert( m_Length >= 0 ) ;
    if ( m_Length < 0 )
        throw DInValException(this) ;
    m_Buffer = new char[m_Length+1] ;
    assert( m_Buffer != NULL ) ;
    m_Buffer[m_Length] = '\0' ;
    if ( m_Buffer == NULL )
        throw DNoMemException() ;
    assert( rhs.m_Buffer != NULL ) ;
    if ( rhs.m_Buffer == NULL )
        throw DInValException(this) ;
    memcpy(static_cast<void *>(m_Buffer), static_cast<const void *>(rhs.m_Buffer), m_Length * sizeof(char)) ;
}

DString& DString::operator+=(const DString & rhs)
{
    char * Temp ;
    m_Length += rhs.m_Length ;
    assert(m_Length >= 0 ) ;
    if ( m_Length < 0 )
        throw DInValException(this) ;
    Temp = new char[m_Length+1] ;
    assert( Temp != NULL ) ;
    if ( Temp == NULL )
        throw DNoMemException() ;
    Temp[m_Length] = '\0' ;
    strcpy(Temp, m_Buffer) ;
    strcat(Temp, rhs.m_Buffer) ;
    if ( m_Buffer != NULL )
        delete [] m_Buffer ;
    m_Buffer = Temp ;
    return *this ;
}

void DString::Deleminate(DString & what)
{
    DPtrList<DString *> List ;
    DString temp(*this) ;
    temp.Tokenizer(what, List) ;
    temp = "" ;
    for ( int i = 0; i < List.GetSize(); i++)
        temp += *(List[i]) ;
    *this = temp ;
}

char DString::DecodeChar(char * what) 
{
      char digit ;

      digit = ( what[0] >= 'A' ? ( ( what[0] & 0xdf ) - 'A' ) + 10 : ( what[0] - '0' ) ) ;
      digit *= 16 ;
      digit += ( what[1] >= 'A' ? ( ( what[1] & 0xdf ) - 'A' ) + 10 : ( what[1] - '0' ) ) ;
      return digit ;
}


void DString::DecodeUrl()
{
    register int i, j ;

    for( i = 0, j = 0; m_Buffer[j]; ++i, ++j) 
    {
        if( ( m_Buffer[i] = m_Buffer[j] ) == '%' ) 
        {
            m_Buffer[i] = DecodeChar( &m_Buffer[j+1] ) ;
            j += 2 ;
        }
    }
    m_Buffer[i] = '\0' ;
    m_Length = i ;
}

void DString::SetBufferAndSize(char * buff, int size)
{
    assert ( buff != NULL ) ;
    if ( buff == NULL )
        throw DNULLPtrAssigmentException(this) ;
    if ( m_Buffer != NULL )
    {
        delete [] m_Buffer ;
        m_Buffer = NULL ;
        m_Length = 0 ;
    }
    m_Buffer = new char[size+1] ;
    if ( m_Buffer == NULL )
        throw DNoMemException() ;
    m_Length = size ;
    memset(m_Buffer,'\0', m_Length+1) ;
    memcpy(m_Buffer, buff, size) ;
}

DString DString::operator+(const DString & rhs) const
{
    DString	Temp = *this ;
    Temp += rhs ;
    return Temp ;
}

char & DString::operator []( int nIndex ) 
{
    if ( !(nIndex > -1 && nIndex <= m_Length) )
        throw DOutOfBoundException(this) ;
    return m_Buffer[nIndex] ;
}

void DString::SetAt(char Value, int nIndex) 
{
    if ( !(nIndex > -1 && nIndex <= m_Length) )
        throw DOutOfBoundException(this) ;
    m_Buffer[nIndex] = Value ;
}

DString DString::ToUpper(void) const
{
    DString Temp = *this ;
    Temp.MakeUpper() ;
    return Temp ;
}

void DString::MakeUpper(void)
{
    assert( m_Length >= 0 ) ;
    for ( int i = 0; i < m_Length; i++)
        if ( (int) m_Buffer[i] >= 97 )
            m_Buffer[i] &= 0xdf ; 
}


bool DString::operator ==(const char * const buff)
{
    if ( m_Buffer == NULL || buff == NULL )
        return false ;
    return ( strcmp(m_Buffer, buff) == 0 ) ;
}

bool DString::operator ==(const DString & rhs)
{
    if ( m_Buffer == NULL || rhs.m_Buffer == NULL )
        return false ;
    return ( strcmp(m_Buffer, rhs.m_Buffer) == 0 ) ;
}

bool DString::operator !=(const char * const buff)
{
    if ( m_Buffer == NULL || buff == NULL )
        return false ;
    return ( strcmp(m_Buffer, buff) != 0 ) ;
}

bool DString::operator !=(const DString & rhs)
{
    if ( m_Buffer == NULL || rhs.m_Buffer == NULL )
        return false ;
    return ( strcmp(m_Buffer, rhs.m_Buffer) != 0 ) ;
}

bool DString::operator >(const DString & rhs)
{
    if ( m_Buffer == NULL || rhs.m_Buffer == NULL )
        return false ;
    return ( strcmp(m_Buffer, rhs.m_Buffer) > 0 ) ;
}

bool DString::operator <(const DString & rhs)
{
    if ( m_Buffer == NULL || rhs.m_Buffer == NULL )
        return false ;
    return ( strcmp(m_Buffer, rhs.m_Buffer) < 0 ) ;
}

bool DString::operator <=(const DString & rhs)
{
    if ( m_Buffer == NULL || rhs.m_Buffer == NULL )
        return false ;
    return ( strcmp(m_Buffer, rhs.m_Buffer) <= 0 ) ;
}

bool DString::operator >=(const DString & rhs)
{
    if ( m_Buffer == NULL || rhs.m_Buffer == NULL )
        return false ;
    return ( strcmp(m_Buffer, rhs.m_Buffer) >= 0 ) ;
}

DString & DString::Format(const char * text, ...)
{
    int n ;
    int oldsize = 100, size = 100 ;
    char * p ;
    va_list val ;
    p = new char[size] ;
    assert( p != NULL ) ;
    if ( p == NULL )
        throw DNoMemException() ;
    for(;;) 
    {
        va_start(val, text) ;
        n = vsnprintf(p, size, text, val) ;
        va_end(val) ;
        if ( n > -1 && n < size ) 
            break ;
        if ( n > -1 ) 
            size = n + 1 ;
        else
            size <<= 1 ; // twice the old size
        char * tmp ;
        tmp = new char[oldsize] ;
        assert( tmp != NULL ) ;
        if ( tmp == NULL )
            throw DNoMemException() ;
        memcpy(tmp, p, oldsize) ;     //can't be interrupted
        delete [] p ;
        p = new char[size] ;
        assert( p != NULL ) ;
        if ( p == NULL )
            throw DNoMemException() ;
        memcpy(p, tmp, oldsize) ;        //can't be interrupted
        delete [] tmp ;
        oldsize = size ;
    }
    if ( p != NULL ) 
    {
        if ( m_Buffer ) 
            delete [] m_Buffer ;
        m_Buffer = p ;
        m_Length = strlen(m_Buffer) ;
    }
    return *this ;
}

void DString::Tokenizer(DString & delim, DPtrList<DString *> & array)
{
    char * temp ;
    DString * Pointer ;
    temp = strtok(m_Buffer, delim.GetBuffer()) ;
    while ( temp ) 
    {
        Pointer = new DString( temp ) ;
        if ( Pointer == NULL )
            throw DNoMemException() ;
        array.AddTail(Pointer) ;
        temp = strtok(NULL, delim.GetBuffer()) ;
    }
}


DString::DString(int size)
{
    if ( size <= 0 ) 
        throw DInValException(this) ;
    m_Buffer = new char[size+1] ;
    assert( m_Buffer != NULL ) ;
    if ( m_Buffer == NULL )
        throw DNoMemException() ;
    m_Length = size ;
}
