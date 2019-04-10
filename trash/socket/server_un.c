#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
    int listenfd, connfd;
    struct sockaddr_un sockaddr;
    char buff[1024];
    int n;

    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sun_family=AF_UNIX;
    strcpy(sockaddr.sun_path, "testPath");

    listenfd=socket(AF_UNIX, SOCK_STREAM, 0);

    bind(listenfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    listen(listenfd, 1024);

    printf("waiting\n");

    while(1)
    {
        if((connfd=accept(listenfd,(struct sockaddr*)NULL,NULL))==-1)
        {
            printf("connect fail\n");
            continue;
        }
        n=read(connfd,buff,1024);
        buff[n]='\0';
        printf("%s",buff);
        close(connfd);
    }
    close(listenfd);
}