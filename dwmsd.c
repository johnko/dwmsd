/* made by johnko 2013-06-10, borrowing from sample server/client script
**
** Mac OS X Compile with:
** gcc -Wall -pedantic -std=c99 dwmsd.c -I/opt/X11/include -L/opt/X11/lib -lc -lX11 -o dwmsd
**
** Notes:
** This program alone does nothing but listen and update XStoreName when told by dwmsc
**
*/
#define _BSD_SOURCE
#define PORT "57475"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <X11/Xlib.h>

static Display *dpy;

void error(char *msg)
{
    perror(msg);
    exit(1);
}

char *
smprintf(char *fmt, ...) //provided by dwmstatus on suckless
{
    va_list fmtargs;
    char *ret;
    int len;
    va_start(fmtargs, fmt);
    len = vsnprintf(NULL, 0, fmt, fmtargs);
    va_end(fmtargs);
    ret = malloc(++len);
    if (ret == NULL) {
        perror("malloc");
        exit(1);
    }
    va_start(fmtargs, fmt);
    vsnprintf(ret, len, fmt, fmtargs);
    va_end(fmtargs);
    return ret;
}

void trim(char *str) //provided by AndiDog on stackoverflow
{
    char *ptr = str;
    while(*ptr == ' ' || *ptr == '\t' || *ptr == '\r' || *ptr == '\n') ++ptr;
    char *end = ptr;
    while(*end) ++end;
    if (end > ptr)
    {
        for(--end; end >= ptr && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'); --end);
    }
    memmove(str, ptr, end-ptr);
    str[end-ptr+1] = 0; //fixed by johnko
} 

void setstatus(char *str) //provided by dwmstatus on suckless
{
    XStoreName(dpy, DefaultRootWindow(dpy), str);
    XSync(dpy, False);
}

int main(int argc, char *argv[]) //provided by http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
{
    int sockfd, newsockfd, portno, clilen, n;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    char *status;
    if (!(dpy = XOpenDisplay(NULL))) {
        fprintf(stderr, "dwmstatus: cannot open display.\n");
        return 1;
    }
    /*
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    //portno = atoi(argv[1]);
    portno = atoi(PORT); //default port
    serv_addr.sin_family = AF_INET;
    //serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //Loopback to be safe!!
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0) 
            error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");
    while(newsockfd)
    {
        bzero(buffer,256);
        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        //printf("Here is the message: %s\n",buffer);
        trim(buffer);
        status = smprintf("%s",buffer);
        setstatus(status);
        n = write(newsockfd,"OK",2);
        if (n < 0) error("ERROR writing to socket");
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
        free(status);
    }
    close(newsockfd);
    close(sockfd);
    XCloseDisplay(dpy);
    return 0; 
}
