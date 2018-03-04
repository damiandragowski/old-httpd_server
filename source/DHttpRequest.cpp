#include "DHttpRequest.h"

DString & DHttpRequest::GetPath()
{
    return path ;
}

void DHttpRequest::DelimPath()
{
    if ( m_pList->GetSize() > 1 )
    {
        DString temp = *((*m_pList)[1]) ;
        DString delim("\r\n") ;
        temp.Deleminate(delim) ;
        if ( temp.IsEmpty() )
        {
            ErrorCode = 404 ;
            return ;
        }
        DString temp3 = temp ;
        DString EndPath("") ;
        delim = "?" ;
        DString filename("index.html") ;
        ext = "html" ;


        DPtrList<DString *> firstArray ;
        DPtrList<DString *> d ;

        bool Yes = false ;
        register int i ;
        // odazru jak jest \r\n\ to zle :)
        

        if ( temp[temp.GetLength()-1] != '/' )
            Yes = true ;

        ErrorCode = 200 ;

        temp3.Tokenizer(delim, firstArray);
        if ( firstArray.GetSize() > 0 )
        {
            temp = *(firstArray[0]) ;
            if ( firstArray.GetSize() > 1 )
                setenv("QUERY_STRING", *(firstArray[1]), 0) ; 
        }

        temp.DecodeUrl() ;

        delim = "/" ;
        temp.Tokenizer(delim, d);

        // index.html ?
        if ( Yes )
        {
            DString file = *(d[d.GetSize()-1]) ;
            DPtrList<DString *> extd ;
            filename = file ;
            delim = "." ;
            file.Tokenizer(delim, extd) ;
            if ( extd.GetSize() == 0 )
            {
                ext = "" ;
            }
            else
            {
                if ( extd.GetSize() == 1 )
                    ext = "" ;
                else
                    ext = *(extd[extd.GetSize()-1]) ;
            }
        }
        for (i = 0; i < d.GetSize(); i++)
        {
            if ( *(d[i]) == ".." )
            {
                if ( i == 0 )
                {
                    // error
                    ErrorCode = 404 ;
                    break;
                }
                else
                {
                    d.RemoveAtCurrent() ;
                    d.RemoveAtCurrent() ;
                }
                i = -1 ;
            }
        }
        if ( ErrorCode == 200 )
        {
            for (i = 0; i < d.GetSize()-1; i++)
            {
                EndPath += (*d[i]) + "/" ;
            }
            if ( EndPath.IsEmpty() )
            {
                if ( filename != "." )
                    EndPath = filename ;
            }
            else
            {
                EndPath += filename ;
            }
        }
        else
        {
            return ;
        }
        if ( EndPath.IsEmpty() )
        {
            ErrorCode = 404 ;
            return ;
        }
        else
        {
            path += EndPath ;
            if ( TFR(access(path.GetBuffer(), R_OK)) == -1 )
                ErrorCode = 404 ;
            else
                ErrorCode = 200 ;
        }
    }
    else
        ErrorCode = 501 ;
}

int DHttpRequest::GetKeepAlive()
{
    register int i ;
    for ( i = 0; i < m_pList->GetSize(); i++) 
    {
        if ( *((*m_pList)[i]) == "Keep-Alive:" )
        {
            if ( i + 1 < m_pList->GetSize() )
                return atoi(*((*m_pList)[i+1])) ;
        }
    }
    return -1 ;
}

bool DHttpRequest::IsKeepAlive()
{
    register int i ;
    for ( i = 0; i < m_pList->GetSize(); i++) 
    {
        if ( *((*m_pList)[i]) == "Connection:" )
        {
            if ( i + 1 < m_pList->GetSize() )
                if ( *((*m_pList)[i+1]) == "keep-alive\r\n" )
                    return true ;
        }
    }
    return false ;
}

DString DHttpRequest::GetMethod()
{
    if ( m_pList->GetSize() > 0 )
    {
        setenv("REQUEST_METHOD", *((*m_pList)[0]), 0) ;
        return *((*m_pList)[0]) ;
    }
    else
    {
        ErrorCode = 405 ;
        return "" ;
    }
}

int DHttpRequest::GetRequestStrings(DSocket & cli, int timeout)
{
    int ret = 0, offset = 0 ;
    DString str("") ;
    DString endline("\r\n") ;
    DString delim(" ") ;

    while ( (ret != -1) && (str != endline) ) 
    {
        ret = cli.ReadLn(timeout, str, ret, offset) ;
        str.Tokenizer(delim, *m_pList) ;
    }
    return ret ;
}

void DHttpRequest::Printf() const
{
    for ( int i=0;i<m_pList->GetSize(); i++)
        (*m_pList)[i]->Printf() ;
}

DHttpRequest::DHttpRequest(DString & _path)
    :path(_path), ErrorCode(200)
{
    DAuto_ptr<DPtrList<DString *> > Temp ( new DPtrList<DString *> ) ;
    m_pList = Temp ;
}

DHttpRequest::~DHttpRequest()
{

}

DString DHttpRequest::GetMime()
{
    DString mime("application/octet-stream") ;
    DString exten = ext.ToUpper() ;
    if ( exten == "HTML" || exten == "HTM" )
    {
        mime = "text/html" ;
    }
    if ( exten == "JPEG" || exten == "JPG" )
    {
        mime = "image/jpeg" ;
    }
    if ( exten == "H" || exten == "CPP" || exten == "TXT" )
    {
        mime = "text/plain" ;
    }
    if ( exten == "GIF" )
    {
        mime = "image/gif" ;
    }
    if ( exten == "PNG" )
    {
        mime = "image/png" ;
    }
    if ( exten == "" && ErrorCode != 200 ) 
    {
        mime = "text/html" ;
    }

    return mime ;
}


