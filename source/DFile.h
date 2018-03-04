#ifndef     DFILE_H
#define     DFILE_H
#define     DFILE_DESCRIPTION         "DFile"

#include "DDescript.h"

class DFile:public DDescript
{
public:
    DFile(DString _path):path(_path) { }
    virtual ~DFile() ;
    int Open(int flag = O_RDONLY) ;
    int GetFileLength() ;
    virtual const char * const ToString() const { return DFILE_DESCRIPTION ; }
protected:
    DString path ;
} ;

#endif  // DFILE_H

