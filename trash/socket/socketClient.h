#include <queue>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/uio.h>


using namespace std;

typedef struct{
    struct sockaddr_un sockaddr;
    int sockfd;
    int cameraid;
}client_params;

typedef struct{
    char* result;
    char* buffer;
}client_data;


typedef enum{
    CMD_SERVER_CLOSE,
    CMD_SERVER_DISCONNECT,

    CMD_GET_AEINFO,
    CMD_GET_GAIN,

}socket_cmd_type;
typedef struct{
    socket_cmd_type cmd;
    void* buffer;
    size_t size;
}msg_packet;

template<typename T>
class Client
{
    public:
        lient();
        virtual ~Client();

        char* begin();
        int funcEnqueue(int(*func)(Client*));
        int setCameraId(int id);

        int setBuffer(char* input);
        int setBuffer(const char* input);
        static int send(Client* client);
        static int recv(Client* client);
        static int recvISPInfo(Client* client);

        int infoDecode(char* input);

        client_data data;
        T* info;

        iovec msgDequeue();
        int testSend();

        pthread_mutex_t msg_queue_mutex;

    private:

        client_params params;
        queue<int(*)(Client*)> funclist;
        queue<msg_packet> msg_queue;
};

//#ifdef __cplusplus
//}
//#endif

template<typename T>
Client<T>::Client()
{

}

template<typename T>
Client<T>::~Client()
{
    free(info);
    free(data.result);
}

template<typename T>
char* Client<T>::begin()
{
    params.sockfd=socket(AF_UNIX,SOCK_STREAM,0);
    memset(&params.sockaddr,0,sizeof(params.sockaddr));
    params.sockaddr.sun_family=AF_UNIX;
    snprintf(params.sockaddr.sun_path, UNIX_PATH_MAX, "*****%d", params.cameraid);
    if((connect(params.sockfd,(struct sockaddr*)&params.sockaddr,sizeof(params.sockaddr)))<0)
    {
        PRINT("Socket Client: connect error %s errno: %d\n",strerror(errno),errno);
        return NULL;
    }
    while(!funclist.empty())
    {
        int (*func)(Client*);
        func=funclist.front();
        funclist.pop();
        int ret = func(this);
        if (ret == -1)
        {
            PRINT("Socket Client: client FUNC error, return:%d",ret);
            return NULL;
        }
    }
    if((write(params.sockfd,"disconn",strlen("disconn"))) < 0)
    {
        PRINT("Socket Client: send disconn error: %s errno : %d",strerror(errno),errno);
    }
    close(params.sockfd);
    return data.result;
}

template<typename T>
int Client<T>::funcEnqueue(int(*func)(Client*))
{
    funclist.push(func);
    return 0;
}

template<typename T>
int Client<T>::setCameraId(int id)
{
    params.cameraid = id;
    return 0;
}

template<typename T>
int Client<T>::setBuffer(char* input)
{
    data.buffer=(char*)input;
    return 0;
}

template<typename T>
int Client<T>::setBuffer(const char* input)
{
    data.buffer=(char*)input;
    return 0;
}

template<typename T>
int Client<T>::send(Client* client)
{
    int ret = write(client->params.sockfd,client->data.buffer,strlen(client->data.buffer));
    return ret;
}

template<typename T>
int Client<T>::recv(Client* client)
{
    char tmp[1024];
    memset(tmp, 0, sizeof(tmp));
    int n = read(client->params.sockfd,tmp,1024);
    if (n == -1)
    {
        client->data.result = NULL;
        return n;
    }
    tmp[n]='\0';
    client->data.result=(char*)malloc(sizeof(tmp));
    if(client->data.result != NULL)
    {
        strcpy(client->data.result,tmp);
        return n;
    }
    else
    {
        PRINT("Socket Client: result malloc failed!");
        return -1;
    }
}

template<typename T>
int Client<T>::recvISPInfo(Client* client)
{
    char tmp[1024];
    memset(tmp, 0, sizeof(tmp));
    int n = read(client->params.sockfd,tmp,1024);
    if(n == -1)
    {
        client->data.result = NULL;
        return n;
    }
    client->data.result = (char*)malloc(sizeof(*info));
    if(client->data.result != NULL)
    {
        memcpy(client->data.result,tmp,sizeof(*info));
        return n;
    }
    else
    {
        PRINT("Socket Client: result malloc failed!");
        return -1;
    }
}

template<typename T>
int Client<T>::infoDecode(char* input)
{
    if (input == NULL)
    {
        info = NULL;
        return -1;
    }
    info = (T*)malloc(sizeof(T));
    if(info!=NULL)
    {
        memcpy(info, input, sizeof(*info));
        return 0;
    }
    else
    {
        PRINT("Socket Client: info malloc failed!");
        return -1;
    }
}

template<typename T>
iovec Client<T>::msgDequeue()
{
    struct iovec msg[1];
    msg[0].iov_base = &msg_queue.pop();
    msg[0].iov_len = sizeof(*(msg[0].iov_base));
    return msg;
}

template<typename T>
int Client<T>::testSend()
{
    pthread_mutex_lock(&msg_queue_mutex);
    iovec msg = msgDequeue();
    pthread_mutex_unlock(&msg_queue_mutex);
    params.sockfd=socket(AF_UNIX,SOCK_STREAM,0);
    memset(&params.sockaddr,0,sizeof(params.sockaddr));
    params.sockaddr.sun_family=AF_UNIX;
    snprintf(params.sockaddr.sun_path, UNIX_PATH_MAX, "******%d", params.cameraid);
    if((connect(params.sockfd,(struct sockaddr*)&params.sockaddr,sizeof(params.sockaddr)))<0)
    {
        PRINT("Socket Client: connect error %s errno: %d\n",strerror(errno),errno);
        return NULL;
    }
    int ret = writev(params.sockfd, &msg, 1);
    if (ret < 0)
    {
        close(params.sockfd);
        return ret;
    }
    recvISPInfo(this);
    if((write(params.sockfd,"disconn",strlen("disconn"))) < 0)
    {
        PRINT("Socket Client: send disconn error: %s errno : %d",strerror(errno),errno);
    }
    close(params.sockfd);
    return data.result;
    return 0;
}