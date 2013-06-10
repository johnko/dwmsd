/* made by johnko 2013-06-10, borrowing from sample server/client script
**
** Mac OS X Compile with:
** gcc -Wall -pedantic -std=c99 dwmsc.c -I/opt/X11/include -L/opt/X11/lib -lc -lX11 -o dwmsc
**
** Notes:
** This program passes strings to dwmsd so it can set XStoreName
**
*/
#define _BSD_SOURCE
#define PORT "57475"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) //provided by http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    /*
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(1);
    }
    */
    //portno = atoi(argv[2]);
    portno = atoi(PORT); //default port
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    //server = gethostbyname(argv[1]);
    server = gethostbyname("127.0.0.1"); //localhost
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    //printf("Please enter the message: ");
    bzero(buffer,256);
    if (argc < 2) {
        fgets(buffer,255,stdin);
    }
    else {
        sscanf(argv[1],"%s",buffer);
    }
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
        error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
        error("ERROR reading from socket");
    //printf("%s\n",buffer);
    return 0;
}
