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
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <X11/Xlib.h>
#include <unistd.h>

static Display *dpy;

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void setstatus(char *str)
{
    XStoreName(dpy, DefaultRootWindow(dpy), str);
    XSync(dpy, False);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
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
    portno = atoi("57475");
    serv_addr.sin_family = AF_INET;
    //serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
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
        printf("Here is the message: %s\n",buffer);
        n = write(newsockfd,"I got your message",18);
        if (n < 0) error("ERROR writing to socket");
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
    }
    close(newsockfd);
    close(sockfd);
    return 0; 
}
