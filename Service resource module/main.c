#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
int times;
int testarray[20];
int testarray1[20]={0,1,2,3,3,3,2,0,3,1,2,1,2,1,2,3,2,1,2,0};
int ResourcesTest[20]={3};
int ImageDisk[20]={0,0,1};
int FtpWeb[20]={2,1};
char CirrOS[]={"CirrOS"};
char WebServer[]={"WebServer"};
char FtpServer[]={"FtpServer"};
char CentOS7[]={"CentOS7"};
int TestNumbers=0;
int main()
{
    ///test
    int whichmethod;
    printf("1.Ftp and Web\n2.Resources are not enough\n3.Image disk space are not enough\n4.Auto Test\n:");
    scanf("%d",&whichmethod);
    if(whichmethod==1)
    {
        TestNumbers=2;
        for(times=0;times<TestNumbers;times++)
        {
            testarray[times]=FtpWeb[times];
        }
    }
    if(whichmethod==2)
    {
        TestNumbers=1;
        for(times=0;times<TestNumbers;times++)
        {
            testarray[times]=ResourcesTest[times];
        }
    }
    if(whichmethod==3)
    {
        TestNumbers=3;
        for(times=0;times<TestNumbers;times++)
        {
            testarray[times]=ImageDisk[times];
        }
    }
    if(whichmethod==4)
    {
        TestNumbers=20;
        for(times=0;times<TestNumbers;times++)
        {
            testarray[times]=testarray1[times];
        }
    }
    for(times=0;times<TestNumbers;times++)
    {
        int sockfd=0;
        ///create socket
        sockfd=socket(AF_INET,SOCK_STREAM,0);
        if(sockfd==-1)
        {
            printf("Fail to create a socket.");
        }
        ///detail of socket
        struct sockaddr_in info;
        bzero(&info,sizeof(info));
        info.sin_family=PF_INET;
        info.sin_addr.s_addr=inet_addr("127.0.0.1");
        info.sin_port=htons(59487);
        int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
        if(err==-1)
        {
            printf("Connection error");
        }
        ///send the Service name
        char receiveMessage[100] = {};
        if(testarray[times]==0)
        {
            send(sockfd,CirrOS,sizeof(CirrOS),0);
        }
        else if(testarray[times]==1)
        {
            send(sockfd,WebServer,sizeof(WebServer),0);
        }
        else if(testarray[times]==2)
        {
            send(sockfd,FtpServer,sizeof(FtpServer),0);
        }
        else if(testarray[times]==3)
        {
            send(sockfd,CentOS7,sizeof(CentOS7),0);
        }
        recv(sockfd,receiveMessage,sizeof(receiveMessage),0);
        printf("%s\n",receiveMessage);
        printf("Close socket\n");
        printf("----------------------------------------------------------\n");
        close(sockfd);
    }

}

