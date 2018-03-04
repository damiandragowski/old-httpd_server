#ifndef     DSEMAPHORE_H
#define     DSEMAPHORE_H
#define     DSEMAPHORE_DESCRIPTION         "DSemaphore"
#define     NM_PROC     10000

#include "DObject.h"
#include "DException.h"
#include "DString.h"
#include "headers.h"

class DSemaphore:public DObject
{
public:
    DSemaphore() ;
    virtual ~DSemaphore() ;
    int Create(key_t key = (key_t)123456L, int initval = 1,int perm = 0666) ;
    int Open(key_t key = (key_t)123456L) ;
    void ChangeVal(int Val) ;
    void Remove() ;
    void Close() ;
    void P() ;
    void V() ;
    virtual const char * const ToString() const { return DSEMAPHORE_DESCRIPTION ; }
protected:
    struct sembuf op_lock[2] ;
    struct sembuf op_endcreate[2] ;
    struct sembuf op_open[1] ;
    struct sembuf op_close[3] ;
    struct sembuf op_unlock[1] ;
    struct sembuf op_op[1] ;
    int    id ;
} ;

#endif  // DSEMAPHORE_H

