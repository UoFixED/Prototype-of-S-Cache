#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
struct tm *PrintfTime;
char TimeBuffer[20];
int main()
{
    int DeleteServerSocket=0,ForClientSocket = 0;
    struct sockaddr_in serverInfo,clientInfo;
    char inputBuffer[256]={};
    int addrlen = sizeof(clientInfo);
    ///create socket
    DeleteServerSocket = socket(AF_INET,SOCK_STREAM,0);
    if (DeleteServerSocket == -1)
    {
        printf("Fail to create a socket.");
    }
    ///detail of socket
    bzero(&serverInfo,sizeof(serverInfo));
    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(9487);
    bind(DeleteServerSocket,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(DeleteServerSocket,5);
    while(1)
    {
        GetTime();
        printf("Delete image server stand by...\n");
        ForClientSocket = accept(DeleteServerSocket,(struct sockaddr*) &clientInfo, &addrlen);
        recv(ForClientSocket,inputBuffer,sizeof(inputBuffer),0);
        char message[256]={};
        ///CirrOS
        if(atoi(inputBuffer)==0)
        {
            system("rm /var/lib/nova/instances/_base/ec9504ab90b325802cb4477bd582d5bdc73b1571");
            strcpy(message,"CirrOS image deleted successfully");
        }
        ///WebServer
        else if(atoi(inputBuffer)==1)
        {
            system("rm /var/lib/nova/instances/_base/4a93b092bb5549ca0b0178c04255edcc6df51138");
            strcpy(message,"WebServer image deleted successfully");
        }
        ///FtpServer
        else if(atoi(inputBuffer)==2)
        {
            system("rm /var/lib/nova/instances/_base/ab3818252d6894b535af33cf8a74a808989a9d80");
            strcpy(message,"FtpServer image deleted successfully");
        }
        ///CentOS7
        else if(atoi(inputBuffer)==3)
        {
            system("rm /var/lib/nova/instances/_base/2123e28a821d55e3b5ff22510681dbb7c8430bfd");
            strcpy(message,"CentOS7 image deleted successfully");
        }
        GetTime();
        printf("%s\n",message);
        send(ForClientSocket,message,sizeof(message),0);
    }
}
void GetTime()
{
    time_t now=time(0);
    PrintfTime=localtime(&now);
    strftime(TimeBuffer,sizeof(TimeBuffer),"%Y-%m-%d %H:%M:%S", PrintfTime);
    printf ("%s  ",TimeBuffer);
}
