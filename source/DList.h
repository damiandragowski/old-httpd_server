#ifndef     DLIST_H
#define     DLIST_H
#define     DLIST_DESCRIPTION		"DList"
#define     DPTRLIST_DESCRIPTION	"DPtrList"
#define     DNODE_DESCRIPTION		"DNode"

#include "DException.h"

template<class D>
class DNode:public DObject
{
public:
	DNode() ;
	DNode(D _Data) ;
	virtual ~DNode() ;
	D & operator=(D & rhs) ;
	DNode<D> *& GetNext() ;
	DNode<D> *& GetPrev() ;
	D & Get() ;
	void Set(D _Data) ;
    virtual const char * const ToString() const { return DNODE_DESCRIPTION ; }	
private:
	DNode<D> * m_pNext ;
	DNode<D> * m_pPrev ;
	D m_DData ;
} ;

template<class D>
class DList:public DObject
{
public:
    virtual const char * const ToString() const { return DLIST_DESCRIPTION ; }	
	DList() ;
	virtual ~DList() ;
	int AddHead(D rhs) throw ( DNoMemException ) ;
	int AddTail(D rhs) throw ( DNoMemException ) ;
	D RemoveHead() throw ( DNULLPtrAssigmentException ) ;
	D RemoveTail() throw ( DNULLPtrAssigmentException ) ;
    virtual void RemoveAtCurrent() throw ( DNULLPtrAssigmentException ) ;
	virtual void RemoveAll() throw( DNULLPtrAssigmentException ) ;
	D operator[](int index) throw ( DOutOfBoundException ) ;
	int GetSize() const ;
protected:
	DNode<D> * m_pHead ;
	DNode<D> * m_pTail ;
	DNode<D> * m_pCurrent ;
	int		   m_iSize ;
	int		   m_iCurrent ;
} ;

template<class D>
class DPtrList:public DList<D>
{
public:
    virtual ~DPtrList() ;
    virtual const char * const ToString() const { return DPTRLIST_DESCRIPTION ; }	
	virtual void RemoveAll() throw( DNULLPtrAssigmentException ) ;
	D & operator[](int index) throw ( DOutOfBoundException ) ;
    virtual void RemoveAtCurrent() throw ( DNULLPtrAssigmentException ) ;
} ;

#include "DString.h"

#ifdef WIN32
#pragma warning( disable : 4290 ) 
#endif

class DString ;

template<class D>
class DAuto_ptr 
{
public:
	explicit DAuto_ptr(D * arg = NULL)
        :m_pPointer(arg)  { }
	template<class U>
        DAuto_ptr(DAuto_ptr<U> & rhs)
            :m_pPointer( rhs.Release() ) { }
	~DAuto_ptr() { delete m_pPointer ; }
    template<class U>
        DAuto_ptr<D> & operator=(DAuto_ptr<U> & rhs)
        {
            if ( this != &rhs ) 
                Reset(rhs.Release()) ;
            return *this ;
        }
    D & operator*() const ;
	D * operator->() const ;
	D * Get() const ;
    D * Release() ;
    void Reset(D * arg = NULL) ;
//    template<class U>
//        friend class DAuto_ptr<U> ;
private:
    D * m_pPointer ;
} ;

template<class D>
inline
void DAuto_ptr<D>::Reset(D * arg)
{
	if ( m_pPointer != arg ) 
	{
		delete m_pPointer ;
		m_pPointer = arg ;
    }
}

template<class D>
inline
D * DAuto_ptr<D>::Release()
{
	D * m_pPointerOld = m_pPointer ;
	m_pPointer = 0 ;
	return m_pPointerOld ;
}

template<class D>
inline
D * DAuto_ptr<D>::Get() const 
{ 
	return m_pPointer ; 
}

template<class D>
inline
D * DAuto_ptr<D>::operator->() const 
{ 
	return m_pPointer ; 
}

template<class D>
inline
D & DAuto_ptr<D>::operator*() const 
{ 
	return *m_pPointer ; 
}

template<class D>
inline
D abs(D arg)
{
	return ( arg >= 0 ) ? arg : -arg ;
}


template<class D>
D & DNode<D>::operator=(D & rhs)
{
	m_DData = rhs ;
	return *this ;
}

template<class D>
void DNode<D>::Set(D _Data)
{
	m_pData = _Data ;
}

template<class D>
D & DNode<D>::Get()
{
	return m_DData ;
}
template<class D>
DNode<D>::~DNode()
{

}

template<class D>
DNode<D> *& DNode<D>::GetPrev()
{
	return m_pPrev ;
}

template<class D>
DNode<D> *& DNode<D>::GetNext()
{
	return m_pNext ;
}

template<class D>
DNode<D>::DNode()
	:m_pNext(NULL), m_pPrev(NULL)
{

}

template<class D>
DNode<D>::DNode(D _Data)
	:m_pNext(NULL), m_pPrev(NULL), m_DData(_Data)
{

}


template<class D>
void DList<D>::RemoveAtCurrent() throw ( DNULLPtrAssigmentException )
{
    assert ( m_pCurrent != NULL ) ;
    if ( m_pCurrent == NULL )
        throw DNULLPtrAssigmentException(this) ;
    if ( m_pHead == m_pTail )
        try
        {
            RemoveHead() ;
        }
        catch ( DNULLPtrAssigmentException & err )
        {
            DString( err.Message() ).Printf() ;
            DString( "\nError on removing current (head)\n" ).Printf() ;
            throw DNULLPtrAssigmentException(this) ;
        }
    else
    {
        if ( m_pCurrent == m_pHead ) 
        {
            try
            {
                RemoveHead() ;
            }
            catch ( DNULLPtrAssigmentException & err )
            {
                DString( err.Message() ).Printf() ;
                DString( "\nError on removing current (head)\n" ).Printf() ;
                throw DNULLPtrAssigmentException(this) ;
            }
        }
        else if ( m_pCurrent == m_pTail) 
        {
            try
            {
                RemoveTail() ;
            }
            catch ( DNULLPtrAssigmentException & err )
            {
                DString( err.Message() ).Printf() ;
                DString( "\nError on removing current (tail)\n" ).Printf() ;
                throw DNULLPtrAssigmentException(this) ;
            }
        }
        else
        {
            DNode<D> * Pointer = m_pCurrent ;
            m_pCurrent->GetNext()->GetPrev() = m_pCurrent->GetPrev() ;
            m_pCurrent->GetPrev()->GetNext() = m_pCurrent->GetNext() ;
            m_pCurrent = m_pCurrent->GetPrev() ;
            delete Pointer ;
            --m_iCurrent ;
            --m_iSize ;
        }
    }
}

template<class D>
int DList<D>::GetSize() const
{
	return m_iSize ;
}

// operator [] ,fast for seq. interations, only one operation each time
template<class D>
D DList<D>::operator[](int index) throw ( DOutOfBoundException ) 
{
	register int i, x, y ;
	assert ( index > -1 && index < m_iSize ) ;
	if ( index < 0 || index >= m_iSize )
		throw DOutOfBoundException(this) ;
	x = abs(index-m_iCurrent) ;
	y = abs(index-(m_iSize-1)) ;
	if ( index >= m_iCurrent )
	{
		if ( x < y )
		{
			for (i = 0; i < x; i++)
				m_pCurrent = m_pCurrent->GetNext() ;
		}
		else
		{
			m_pCurrent = m_pTail ;
			for (i = 0; i < y; i++)
				m_pCurrent = m_pCurrent->GetPrev() ;
		}
	} 
	else 
	{
		if ( x < index )
		{
			for (i = 0; i < x; i++)
				m_pCurrent = m_pCurrent->GetPrev() ;
		}
		else
		{
			m_pCurrent = m_pHead ;
			for (i = 0; i < index; i++)
				m_pCurrent = m_pCurrent->GetNext() ;
		}
	}
	m_iCurrent = index ;
	return m_pCurrent->Get() ;
}

template<class D>
D DList<D>::RemoveTail() throw ( DNULLPtrAssigmentException ) 
{
	if ( m_pTail == NULL )
		throw DNULLPtrAssigmentException(this) ;
	DNode<D> * Pointer = m_pTail ;
	D Temp = Pointer->Get() ;
	--m_iSize ;
	if ( m_pHead == m_pTail )
	{
		m_pHead = m_pTail = m_pCurrent = NULL ;
	}
	else
	{
		m_pTail->GetPrev()->GetNext() = NULL ;
		if ( m_pCurrent == m_pTail )
		{
			m_pCurrent = m_pTail = m_pTail->GetPrev() ;
			--m_iCurrent ;
		}
		else
		{
			m_pTail = m_pTail->GetPrev() ;
		}
	}
	delete Pointer ;
	return Temp ;
}

template<class D>
D DList<D>::RemoveHead() throw ( DNULLPtrAssigmentException ) 
{
	if ( m_pHead == NULL )
		throw DNULLPtrAssigmentException(this) ;
	DNode<D> * Pointer = m_pHead ;
	D Temp = Pointer->Get() ;
	--m_iSize ;
	if ( m_pHead == m_pTail )
	{
		m_pHead = m_pTail = m_pCurrent = NULL ;
	}
	else
	{
		m_pHead->GetNext()->GetPrev() = NULL ;
		if ( m_pCurrent == m_pHead )
		{
			m_pCurrent = m_pHead = m_pHead->GetNext() ;			
		}
		else
		{
			m_pHead = m_pHead->GetNext() ;
			--m_iCurrent ;
		}
	}
	delete Pointer ;
	return Temp ;
}

template<class D>
int DList<D>::AddHead(D rhs) throw ( DNoMemException )
{
	DNode<D> * Pointer = new DNode<D>(rhs) ;
	if ( Pointer == NULL )
		throw DNoMemException() ;
	++m_iSize ;
	if ( m_pHead == NULL )
	{
		m_pCurrent = m_pHead = m_pTail = Pointer ;
	}
	else
	{
		Pointer->GetNext() = m_pHead ;
		m_pHead->GetPrev() = Pointer ;
		m_pHead = Pointer ;
		++m_iCurrent ;
	}
	return 0 ;
}

template<class D>
int DList<D>::AddTail(D rhs) throw ( DNoMemException )
{
	DNode<D> * Pointer = new DNode<D>(rhs) ;
	if ( Pointer == NULL )
		throw DNoMemException() ;
	++m_iSize ;
	if ( m_pTail == NULL )
	{
		m_pHead = m_pTail = m_pCurrent = Pointer ;
	}
	else
	{
		Pointer->GetPrev() = m_pTail ;
		m_pTail->GetNext() = Pointer ;
		m_pTail = Pointer ;
	}
	return 0 ;
}

template<class D>
void DList<D>::RemoveAll() throw( DNULLPtrAssigmentException )
{
	register int i, size = m_iSize ;
	try
	{
		for ( i = 0; i < size; i++)
			RemoveHead() ;
	}
	catch ( DNULLPtrAssigmentException & err )
	{
		DString( err.Message() ).Printf() ;
		DString( "\nError in DList RemoveHead()\n" ).Printf() ;
		throw DNULLPtrAssigmentException(this) ;
	}
}

template<class D>
DList<D>::~DList()
{
	try
	{
		RemoveAll() ;
	}
	catch ( DNULLPtrAssigmentException & err )
	{
		DString( err.Message() ).Printf() ;
		DString( "\nError in DList RemoveAll()\n" ).Printf() ;
		throw DNULLPtrAssigmentException(this) ;
	}
}

template<class D>
DList<D>::DList()
	:m_pHead(NULL), m_pTail(NULL), m_pCurrent(NULL), m_iSize(0), m_iCurrent(0)
{

}

template<class D>
DPtrList<D>::~DPtrList()
{
	try
	{
		RemoveAll() ;
	}
	catch ( DNULLPtrAssigmentException & err )
	{
		DString( err.Message() ).Printf() ;
		DString( "\nError in DList RemoveAll()\n" ).Printf() ;
		throw DNULLPtrAssigmentException(this) ;
	}
}


template<class D>
void DPtrList<D>::RemoveAll() throw( DNULLPtrAssigmentException )
{
	register int i, size = m_iSize ;
	try
	{
		for ( i = 0; i < size; i++)
			delete RemoveHead() ;
	}
	catch ( DNULLPtrAssigmentException & err )
	{
		DString( err.Message() ).Printf() ;
		DString( "\nError in DList RemoveHead()\n" ).Printf() ;
		throw DNULLPtrAssigmentException(this) ;
	}
}

template<class D>
D & DPtrList<D>::operator[](int index) throw ( DOutOfBoundException ) 
{
	register int i, x, y ;
	assert ( index > -1 && index < m_iSize ) ;
	if ( index < 0 || index >= m_iSize )
		throw DOutOfBoundException(this) ;
	x = abs(index-m_iCurrent) ;
	y = abs(index-(m_iSize-1)) ;
	if ( index >= m_iCurrent )
	{
		if ( x < y )
		{
			for (i = 0; i < x; i++)
				m_pCurrent = m_pCurrent->GetNext() ;
		}
		else
		{
			m_pCurrent = m_pTail ;
			for (i = 0; i < y; i++)
				m_pCurrent = m_pCurrent->GetPrev() ;
		}
	} 
	else 
	{
		if ( x < index )
		{
			for (i = 0; i < x; i++)
				m_pCurrent = m_pCurrent->GetPrev() ;
		}
		else
		{
			m_pCurrent = m_pHead ;
			for (i = 0; i < index; i++)
				m_pCurrent = m_pCurrent->GetNext() ;
		}
	}
	m_iCurrent = index ;
	return m_pCurrent->Get() ;
}

template<class D>
void DPtrList<D>::RemoveAtCurrent() throw ( DNULLPtrAssigmentException )
{
    assert ( m_pCurrent != NULL ) ;
    if ( m_pCurrent == NULL )
        throw DNULLPtrAssigmentException(this) ;
    if ( m_pHead == m_pTail )
        try
        {
            delete RemoveHead() ;
        }
        catch ( DNULLPtrAssigmentException & err )
        {
            DString( err.Message() ).Printf() ;
            DString( "\nError on removing current (head)\n" ).Printf() ;
            throw DNULLPtrAssigmentException(this) ;
        }
    else
    {
        if ( m_pCurrent == m_pHead ) 
        {
            try
            {
                delete RemoveHead() ;
            }
            catch ( DNULLPtrAssigmentException & err )
            {
                DString( err.Message() ).Printf() ;
                DString( "\nError on removing current (head)\n" ).Printf() ;
                throw DNULLPtrAssigmentException(this) ;
            }
        }
        else if ( m_pCurrent == m_pTail) 
        {
            try
            {
                delete RemoveTail() ;
            }
            catch ( DNULLPtrAssigmentException & err )
            {
                DString( err.Message() ).Printf() ;
                DString( "\nError on removing current (tail)\n" ).Printf() ;
                throw DNULLPtrAssigmentException(this) ;
            }
        }
        else
        {
            DNode<D> * Pointer = m_pCurrent ;
            m_pCurrent->GetNext()->GetPrev() = m_pCurrent->GetPrev() ;
            m_pCurrent->GetPrev()->GetNext() = m_pCurrent->GetNext() ;
            m_pCurrent = m_pCurrent->GetPrev() ;
            delete  Pointer->Get() ;
            delete  Pointer ;
            --m_iCurrent ;
            --m_iSize ;
        }
    }
}

template<class D>
inline
D Max(D a, D b)
{
    return ( a > b ) ? a : b ;
}

template<class D>
inline
D Min(D a, D b)
{
    return ( a < b ) ? a : b ;
}


#endif  // DLIST_H

