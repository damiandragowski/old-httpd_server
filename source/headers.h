#ifndef		HEADERS_H
#define		HEADERS_H

#define TFR(expression)\
(__extension__({\
    long int tfrx_result;\
    do\
    {\
        tfrx_result=(long int)(expression);\
    }while(tfrx_result==-1L && errno==EINTR);\
    tfrx_result;\
})) 

#define TFRVOID(expression)\
(__extension__({\
    do\
    {\
        errno = 0 ;\
        (expression);\
    }while(errno==EINTR);\
})) 


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef SIGTSTP
#include <sys/file.h>
#include <sys/ioctl.h>
#endif



#endif	// HEADERS_H
