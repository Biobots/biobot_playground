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

using namespace std;
//decltype int(*func)(ISP_Client*);

typedef struct{
    struct sockaddr_un sockaddr;
    int sockfd;
    int cameraid;
}isp_client_params;

typedef struct{
    char* result;
    char* buffer;
}isp_client_data;

class ISP_Client
{
    public:
        ISP_Client();
        virtual ~ISP_Client();

        int begin();
        int funcEnqueue(int(*func)(ISP_Client*));

        int setBuffer(char* input);
        int setBuffer(const char* input);
        static int send(ISP_Client* client);
        static int recv(ISP_Client* client);
        //int getResult();

        isp_client_data isp_data;
        isp_client_params isp_params;
    private:

        
        queue<int(*)(ISP_Client*)> funclist;
};