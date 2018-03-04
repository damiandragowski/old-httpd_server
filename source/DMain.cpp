#include "DServerd.h"

class DMain
{
public:
    DMain(int argc, char **argv) 
    {
        try
        {
            DServerd ServerHTTP ;
            if ( ServerHTTP.Parsing(argc, argv) != -1 )
                ServerHTTP.Run() ;
        }
        catch ( DException & err )
        {
            printf( err.Message() ) ;
            printf("\nError in Application. It's going to Terminate\n") ;
        }
    }
    ~DMain() { }
} ;



int main(int argc, char ** argv)
{
    DMain MainClass(argc,argv) ;
    sleep(1) ;
    return 0 ;
}


    /*
    DString path("/www/") ;


    DSocket sock ;
    DNetAddress addr(PF_INET, 80, "") ;
    sock.Bind(addr) ;
    sock.Listen() ;

    int timeout = 3*60*1000 ;
    int len, ret, ErrorCode = 200 ;


    


    // tu while
    while(1)
    {
        DSocket cli = sock.Accept() ;
        DHttpRequest httpreq(path) ;

        httpreq.GetRequestStrings(cli, timeout) ;  // jesli -1 to timeout lub blad :), pozatym wyjatki

        if ( httpreq.IsKeepAlive() )
        {
            if ( ( ret = httpreq.GetKeepAlive()) != -1 )
            {
                timeout = ret ;
            }
        }
    
        if ( httpreq.GetMethod() != "GET" )
        {
            ErrorCode = 405 ;
        }

        httpreq.DelimPath() ;

        if ( ( ret = httpreq.GetErrorCode() ) != 200 )
        {
            ErrorCode = ret ;
        }

        DFile file(httpreq.GetPath()) ;
        file.Open() ;
        len = file.GetFileLength() ;

        DString xxx ;
        xxx.Format("HTTP/1.1 200\nConnection: Closed\nContent-type: text/html\nContnet-Lenght: %d\n\n", len) ;
        len = xxx.GetLength() ;
        cli.Write((const void *) xxx.GetBuffer(), len ) ;

        while( (len = file.Read()) != 0 )
        {

            int size = len, tmp1 = 0, tmp ;
            while ( size != 0 )
            {
                tmp = cli.Write((const void *) (file.getBuffer()+tmp1), size ) ;
                tmp1 += tmp ;
                size -= tmp ;
            }
        }

        file.Close() ;
        cli.ShutDown() ;
        cli.Close() ;


    }
    sock.Close() ;

*/





    /*
    

    DString absPath("/www/../httpd.tar") ;
    DString ret("/") ;
    DPtrList<DString *> d ;
    DString delim("/") ;
    absPath.Tokenizer(delim,d) ;

    for ( int i = 0; i < d.GetSize();i++ ) 
    {
        if ( *(d[i]) != ".." )
            ret += *(d[i]) + "/" ;
    }
   


    d.RemoveAll() ;
    ret.Printf() ;

    
    
    
    DAuto_ptr<DList<int> > lista ( new DList<int> ) ;
    lista->AddHead(1) ;
    printf("%d\n", (*lista)[0] ) ;




    DFork proc ;
    proc.Fork() ;
    if ( proc.isChild()) 
    {
        printf("dziecko\n") ;
    }
    else
    {
        printf("Matka\n") ;
        TFRVOID(sleep(10)) ;
    }
    
    
        try
        {
        DString path("/www") ;
        DSocket sock1 ;
        DSocket sock2 ;
        DNetAddress addr1(PF_INET, 80, "127.0.0.1") ;
        DNetAddress addr2(PF_INET, 81, "127.0.0.1") ;
        sock1.Bind(addr1) ;
        sock1.Listen() ;
        sock2.Bind(addr2) ;
        sock2.Listen() ;



        DString xxx ;
        char * buff = new char[1001] ;



        while(1)
        {
            moj.P() ;
            DSocket cli = sock.Accept() ;
            moj.V() ;
            int len = cli.Read((void *) buff, 1000) ;
            buff[1000] = '\0' ;
            DString( buff ).Printf() ;
            xxx.Format("%s%s%s", "HTTP/1.1 200\nContent-type: text/html\nConnection: Closed\n","<html><head><title>ania</title></head><body>\n","<h1>Ania Bania</h1></body></html>") ;

            len = xxx.GetLength() ;
            cli.Write((const void *) xxx.GetBuffer(), len ) ;


            cli.ShutDown() ;
            cli.Close() ;
        } 
    }
        catch (DSystemException & err )
        {
            printf("%s\n", err.Message()) ;
        }
        catch (DNULLPtrAssigmentException & err )
        {
            printf("%s\n", err.Message()) ;
        }
        catch (DNoMemException & err )
        {
            printf("%s\n", err.Message()) ;
        }
        catch (DInValException & err )
        {
            printf("%s\n", err.Message()) ;
        }
        catch (DOutOfBoundException & err )
        {
            printf("%s\n", err.Message()) ;
        }
            
            
            
            DFork proc ;

            // init server :)

            DSocket sock ;
            DNetAddress addr(PF_INET, 80, "") ;
            sock.Bind(addr) ;
            sock.Listen() ;


            DSocket cli = sock.Accept() ;
            DString str("") ;
            
            int ret = 0, offset = 0 ;
            DString endline("\r\n") ;
            DString str1 ;
            DString h ;
            DPtrList<DString *> array ;
            DString delim(" ") ;
            while ( (ret != -1) && (str != endline) ) 
            {
                ret = cli.ReadLn(60*1000, str, ret, offset) ;
                str1 = str ;
                str1.Tokenizer(delim, array) ;
            }
            
            int i ;
            for ( i=0;i<array.GetSize();i++) 
            {
                if ( *(array[i]) == "Connection:" )
                {
                    break;
                }
            }
            if ( i != array.GetSize() )
            {
                array[i+1]->Printf() ;
                printf("i = %d\n", i) ;
            }
            printf("\n") ;
            array.RemoveAll() ;
            cli.ShutDown() ;
            cli.Close() ;
            sock.Close() ;
        }
        catch (DSystemException & err )
        {
            printf("%s\n", err.Message()) ;
        }
        catch (DNULLPtrAssigmentException & err )
        {
            printf("%s\n", err.Message()) ;
        }
        catch (DNoMemException & err )
        {
            printf("%s\n", err.Message()) ;
        }
        catch (DInValException & err )
        {
            printf("%s\n", err.Message()) ;
        }
        catch (DOutOfBoundException & err )
        {
            printf("%s\n", err.Message()) ;
        }     




            // zawsze n-1 to bedzie make n proc

            // a tu Run section
            
            DString xxx ;
            DFile file("/car15.jpg") ;
//            DFile file2("/car2.jpg") ;
            file.Open() ;
  //          file2.Open(O_CREAT|O_WRONLY) ;




            while(1)
            {

                DSocket cli = sock.Accept() ;

                int len = cli.Read(BUFFERSIZE) ;

                DString( cli.getBuffer() ).Printf() ;


                //DString( buff ).Printf() ;

                xxx.Format("HTTP/1.1 200\nConnection: close\nContent-length: 42577\nContent-type: image/jpeg\n\n") ;
                len = xxx.GetLength() ;
                cli.Write((const void *) xxx.GetBuffer(), len ) ;
                
                int reads ;
                while ( (reads = file.Read( BUFFERSIZE )) != 0 )
                {
                    cli.Write(file.getBuffer(), reads) ;
//                    file2.Write((void *)buff, reads) ;
                }

                cli.ShutDown() ;
                cli.Close() ;
            } 
        }
        catch (DSystemException & err )
        {
            printf("%s\n", err.Message()) ;
        }
        catch (DNULLPtrAssigmentException & err )
        {
            printf("%s\n", err.Message()) ;
        }
        catch (DNoMemException & err )
        {
            printf("%s\n", err.Message()) ;
        }
        catch (DInValException & err )
        {
            printf("%s\n", err.Message()) ;
        }
        catch (DOutOfBoundException & err )
        {
            printf("%s\n", err.Message()) ;
        }     

    
    DSharedMem mem ;
    DSemaphore sem ;
    sem.Create() ;
    mem.Create() ;
    char * text = mem.GetPermToBuffer() ;
    memset((void *) text, '\0', 100) ;
    if (fork() == 0) 
    {
        while(1)
        {
            sleep(1) ;
            sem.P() ;
            for(int i=0;i<20;i++)
            {
                text[i] = 'a' + i ;
                if (i==19)
                    text[i] = '\n' ;
            }
            sem.V() ;
        }
    }
    else
    {
        while(1)
        {
            sem.P() ;
            if ( text[0] != '\0' ) 
                printf(text) ;
            for(int i=0;i<20;i++)
            {
                text[i] = '\0' ;
            }
            sem.V() ;
        }
    }
    
    
    DSemaphore sem ;
    sem.Create() ;
    if (fork() == 0 ) 
    {

        while (1)
        {
            printf("Dziekco\n") ;
            sem.P() ;
        }
    }
    else
    {
        while (1)
        {
            printf("Matka\n") ;
            sem.P() ;
        }
    }
    
    
    
    
    
    try
    {
        DServerd server ;
        if ( server.Parsing(argc, argv) != -1 )
            server.Run() ;
    } 
    catch (DException err)
    {
        printf( err.Message() ) ;
    }
    



    DSocket sock ;
    DNetAddress addr(PF_INET, 80, "212.77.100.101") ;
    try
    {
        sock.Connect(addr) ;
    }
    catch( DException err )
    {

        DString( "Connection failed\n" ).Printf() ;
    }

    DString getstring( "GET /\n\r\n\r" ) ;
    int size = getstring.GetSize() ;
    sock.Write((const void *)getstring.GetBuffer(), size) ;
    char * text = new char[1001] ;
    text[1000] = '\0' ;
    sock.Read((void *)text ,999) ;
    DString( text ).Printf() ;
    delete [] text ;
    sock.Close() ;


    
    
    DFile ania("/ania.txt") ;
    try
    {
        ania.Open() ;
    }
    catch ( DInValException err )
    {
        DString( err.Message() ).Printf() ;
        DString( "Can't open file" ).Printf() ;
    }
    int len = ania.Read(100) ;
    DString( ania.getBuffer() ).Printf() ;
    DString str ;
    str.Format("%d\n", len) ;
    str.Printf() ;



    
    DString xxx ;
    DSocket sock ;
    DNetAddress addr(PF_INET, 80, "127.0.0.1") ;
    sock.Bind(addr) ;
    sock.Listen() ;
    char * buff = new char[1001] ;
    while(1)
    {
        DSocket cli = sock.Accept() ;
        int len = cli.Read((void *) buff, 1000) ;
        buff[1000] = '\0' ;
        DString( buff ).Printf() ;
        xxx.Format("\n%d\n", len) ;
        xxx.Printf() ;
        cli.ShutDown() ;
        cli.Close() ;
    }
    */





