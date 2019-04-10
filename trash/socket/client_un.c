#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main(int argc, char** argv)
{
    int socketfd;
    struct sockaddr_un sockaddr;
    char buff[1024];
    int n;

    if (argc==2)
    {
        printf("client <ipaddress> \n");
        exit(0);
    }

    socketfd=socket(AF_UNIX,SOCK_STREAM,0);
    memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sun_family=AF_UNIX;
    strcpy(sockaddr.sun_path,"testPath");
    if((connect(socketfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr)))<0)
    {
        printf("connect error %s errno: %d\n",strerror(errno),errno);
        exit(0);
    }
    printf("send message to server\n");
    fgets(buff,1024,stdin);
    if((send(socketfd,buff,strlen(buff),0)) < 0)
    {
        printf("send mes error: %s errno : %d",strerror(errno),errno);
        exit(0);
    }
    close(socketfd);
    printf("exit\n");
    exit(0);
}