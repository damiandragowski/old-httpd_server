#ifndef     DSHAREDMEM_H
#define     DSHAREDMEM_H
#define     DSHAREDMEM_DESCRIPTION         "DSharedMem"

#include "DObject.h"
#include "DException.h"
#include "headers.h"


template<class D>
class DSharedMem:public DObject
{
public:
    DSharedMem() ;
    virtual ~DSharedMem() ;
    int Create(key_t key = (key_t)23456L, int shmflag = 0666) ;
    int Open(key_t key = (key_t)23456L) ;
    D * GetPermToBuffer() ;
    D * GetBuffer() const { return m_pBuffer ; } 
    int Close() ;
    int Destory() ;
    virtual const char * const ToString() const { return DSHAREDMEM_DESCRIPTION ; }
protected:
    int id ;
    D * m_pBuffer ;
} ;

template<class D>
DSharedMem<D>::DSharedMem()
{
    id = -1 ;
}

template<class D>
DSharedMem<D>::~DSharedMem()
{
    Close() ;
}

template<class D>
int DSharedMem<D>::Create(key_t key, int shmflag)
{
    if ( (id = TFR(shmget(key, sizeof(D), shmflag | IPC_CREAT))) < 0 )
        throw DSystemException(this) ;
    return id ;
}

template<class D>
int DSharedMem<D>::Open(key_t key)
{
    if ( (id = TFR(shmget(key, sizeof(D), 0))) < 0 )
        throw DSystemException(this) ;
    return id ;
}

template<class D>
D * DSharedMem<D>::GetPermToBuffer()
{
    if ( (m_pBuffer = (D *)TFR(shmat(id, (char *) 0, 0))) == (D *) -1 )
        throw DSystemException(this) ;
    return m_pBuffer ;
}

template<class D>
int DSharedMem<D>::Destory()
{
    int ret ;
    if ( (ret = TFR(shmctl(id, IPC_RMID, (struct shmid_ds *)0))) < 0 )
        throw DSystemException(this) ;
    return ret ;
}

template<class D>
int DSharedMem<D>::Close()
{
    int ret ;
    if ( (ret = (int) TFR(shmdt(m_pBuffer))) < 0 )
        throw DSystemException(this) ;
    return ret ;
}

#endif  // DSHAREDMEM_H

