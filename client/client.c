/* 
*client.c 
*overred
*notice:file's tail(return 0)will cause client to exit
*the server will be getting a SIGPIPE signal while write buffer has unsent stuff
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "common.h"



int main(int argc, char *argv[])
{

    char *host="127.0.0.1";
    int port=5372;
    struct sockaddr_in servaddr;
    
    
    //char buf[MAXLINE]="a0001\0 a0001\0 abcdefg\0";
    char buf[MAXLINE]="0000000001000000000101276878898932994324234325435sdfdsf";
    
    
    int sockfd;
    
      /* ============sockfd============ */
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(host);
    servaddr.sin_port = htons(port);
    /* ============connect============ */
    Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));


    while(1)
    {
       if(write(sockfd,buf,strlen(buf))==-1){
         printf("write error\n");
       }
       sleep(1);
      // printf("write %lu\n",strlen(buf));

        int length = read(sockfd, buf, MAXLINE);

        if (length>0) {
            printf("return %s\n",buf+20);
        }

    }

    return 0;
}

