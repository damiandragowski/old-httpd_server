#include "DSemaphore.h"

DSemaphore::DSemaphore()
{
    op_lock[0].sem_num = 2 ;
    op_lock[0].sem_op = 0 ;
    op_lock[0].sem_flg = 0 ;
    op_lock[1].sem_num = 2 ;
    op_lock[1].sem_op = 1 ;
    op_lock[1].sem_flg = SEM_UNDO ;

    op_endcreate[0].sem_num = 1 ;
    op_endcreate[0].sem_op = -1 ;
    op_endcreate[0].sem_flg = SEM_UNDO ;
    op_endcreate[1].sem_num = 2 ;
    op_endcreate[1].sem_op = -1 ;
    op_endcreate[1].sem_flg = SEM_UNDO ;

    op_open[0].sem_num = 1 ;
    op_open[0].sem_op = -1 ;
    op_open[0].sem_flg = SEM_UNDO ;

    op_close[0].sem_num = 2 ;
    op_close[0].sem_op = 0 ;
    op_close[0].sem_flg = 0 ;
    op_close[1].sem_num = 2 ;
    op_close[1].sem_op = 1 ;
    op_close[1].sem_flg = SEM_UNDO ;
    op_close[2].sem_num = 1 ;
    op_close[2].sem_op = 1 ;
    op_close[2].sem_flg = SEM_UNDO ;

    op_unlock[0].sem_num = 2 ;
    op_unlock[0].sem_op = -1 ;
    op_unlock[0].sem_flg = SEM_UNDO ;

    op_op[0].sem_num = 0 ;
    op_op[0].sem_op = 99 ;
    op_op[0].sem_flg = SEM_UNDO ;

    id = -1 ;
}

DSemaphore::~DSemaphore()
{
    if ( id != -1 ) 
        Close() ;
}

int DSemaphore::Create(key_t key, int initval, int perms)
{
    register int semval ;
    bool         escape = true ;
    union semun 
    {
        int val ;
        struct semid_ds * buf ;
        ushort * array ;
    } semctl_arg ;
    if ( key == IPC_PRIVATE || key == (key_t)-1 ) 
        throw DInValException(this) ;

    while ( escape ) 
    {
        escape = false ;
        if ( (id = TFR(semget(key, 3, perms | IPC_CREAT ))) < 0 ) 
            throw DInValException(this) ;

        if ( TFR(semop(id, &op_lock[0], 2)) < 0)
            if ( errno == EINVAL )
                escape = true ;
            else 
                throw DInValException(this) ; // we have more trouble

    }
    if ((semval = TFR(semctl(id, 1, GETVAL, 0))) < 0 )
        throw DSystemException(this) ;

    if ( semval == 0 ) 
    {
        semctl_arg.val = initval ;
        if ( TFR(semctl(id, 0, SETVAL, semctl_arg)) < 0 )
            throw DSystemException(this) ;

        semctl_arg.val = NM_PROC ;
        if ( TFR(semctl(id, 1, SETVAL, semctl_arg)) < 0 )
            throw DSystemException(this) ;

    }
    if ( TFR(semop(id, &op_endcreate[0], 2)) < 0 ) 
        throw DSystemException(this) ;

    return id ;
}

int DSemaphore::Open(key_t key)
{
    if ( key == IPC_PRIVATE || key == (key_t)-1 ) 
        throw DInValException(this) ;

    if ( (id = TFR(semget(key, 3, 0))) < 0 )
        throw DSystemException(this) ;

    if ( TFR(semop(id, &op_open[0], 1)) < 0 ) 
        throw DSystemException(this) ;

    return id ;
}

void DSemaphore::Remove()
{
    if ( TFR(semctl(id, 0, IPC_RMID, 0)) < 0 )
        throw DSystemException(this) ;
    id = -1 ;
}

void DSemaphore::Close()
{
    register int semval ;

    if ( TFR(semop(id, &op_close[0], 3)) < 0 ) 
        throw DSystemException(this) ;

    if ((semval = TFR(semctl(id, 1, GETVAL, 0))) < 0 )
        throw DSystemException(this) ;

    if ( semval > NM_PROC ) 
        throw DSystemException(this) ;
    else if ( semval == NM_PROC )
        Remove() ;
    else
        if ( TFR(semop(id, &op_unlock[0], 1)) < 0 ) 
            throw DSystemException(this) ;
    id = -1 ;
}

void DSemaphore::ChangeVal(int Val)
{
    if ( ( op_op[0].sem_op = Val ) == 0 ) 
        throw DSystemException(this) ;

    if ( TFR(semop(id, &op_op[0], 1)) < 0 ) 
        throw DSystemException(this) ;
}

void DSemaphore::P()
{
    try
    {
        ChangeVal(-1) ;
    }
    catch( DSystemException err )
    {
        DString( "\nFunction P error\n" ).Printf() ;
        DString( err.Message() ).Printf() ;
        throw DSystemException(this) ;
    }
}

void DSemaphore::V()
{
    try
    {
        ChangeVal(1) ;
    }
    catch( DSystemException err )
    {
        DString( "\nFunction V error\n" ).Printf() ;
        DString( err.Message() ).Printf() ;
        throw DSystemException(this) ;
    }
}
