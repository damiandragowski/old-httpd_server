#include "DFile.h"

DFile::~DFile()
{
    
}

int DFile::Open(int flag)
{
    fd = TFR(open(path.GetBuffer(), flag)) ;
    if ( fd == -1 )
        throw DInValException(this) ;
    return fd ;
}

int DFile::GetFileLength()
{
    if ( fd == -1 )
        throw DInValException(this) ;
    struct stat _stats ;
    if ( TFR( stat( path.GetBuffer(), (struct stat *) &_stats)) < 0 )
        throw DSystemException(this) ;
    return  _stats.st_size ;
}
