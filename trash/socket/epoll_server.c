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
#include <sys/epoll.h>

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

    

    int epfd=epoll_create(256);
    int nfds;
    struct epoll_event ev,events[20];
    ev.events=EPOLLIN;
    ev.data.fd=listenfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    bind(listenfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    listen(listenfd, 1024);
    do
    {
        nfds=epoll_wait(epfd,events,20,-1)
        for(i=0;i<nfds,i++)
        {
            memset(buff,0,sizeof(buff));
            if(events[i].data.fd==listenfd)
            {
                connfd=accept(listenfd,(struct sockaddr*)NULL,NULL);
                ev.data.fd=connfd;
                ev.events=EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
            }
            else if(events[i].events&EPOLLIN)
            {
                n=read(events[i].data.fd,buff,1024);
                if(strcmp(buff, "exit")==0)
                {
                    running = 0;
                    break;
                }
                else if(strcmp(buff,"disconn")==0)
                {
                    epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd,&events[i]);
                    close(events[i].data.fd);
                }
                else
                {
                    ev.data.fd=events[i].data.fd;
                    ev.events=EPOLLOUT;
                    epoll_ctl(epfd,EPOLL_CTL_MOD,events[i].data.fd,&ev);
                }
            }
            else if(events[i].events&EPOLLOUT)
            {
                write(events[i].data.fd,"test",strlen("test"));
                ev.data.fd=events[i].data.fd;
                ev.events=EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_MOD,events[i].data.fd,&ev);
            }
        }
    }while(running);
    close(epfd);
    close(listenfd);
    unlink(sockaddr.sun_path);
}