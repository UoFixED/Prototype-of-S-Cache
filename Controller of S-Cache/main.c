#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define ServiceCount 4
#define CirrOS 0
#define WebServer 1
#define FtpServer 2
#define CentOS7 3
#define KeyRed "\x1B[31m"
#define ResetColor "\x1b[0m"
///For VM instance
double JobHitEdge[ServiceCount]={};
double JobHitPast[ServiceCount]={};
double JobTimer[ServiceCount]={};
int CacheStatus[ServiceCount]={};

///For Image
double ImageHit[ServiceCount]={};
double Clock=0;
int ImageStatus[ServiceCount]={};

///For cloud resources
int CurrentCpu=0,CurrentRam=0,CurrentDisk=0;
int QuotaCpu=8,QuotaRam=16339,QuotaDisk=40;
int CurrentImageDisk=20;

///detail of Services and Images
int ServiceResources[ServiceCount][3]={{1,1024,1},{1,2048,5},{1,2048,5},{2,4096,10}};
double NorServiceResources[ServiceCount][3]={{0.125,0.0625,0.02},{0.125,0.125,0.1},{0.125,0.125,0.1},{0.25,0.25,0.2}};
double ImageResources[ServiceCount]={0.011,1.92,1.68,3.68};

///output and control error
double HitCounter=0,RequestCounter=0;
int FlagForSecondTime=0;

///log write
FILE *LogFiles;
char TimeBuffer[20];
struct tm *PrintfTime;

void main()
{
    int MainServerSocket=0;
    int GetClientSocket=0;
    struct sockaddr_in MainServerInfo,MainClientInfo;
    int addrlen = sizeof(MainClientInfo);
    ///create socket
    MainServerSocket = socket(AF_INET,SOCK_STREAM,0);
    if (MainServerSocket == -1)
    {
        GetTime();
        printf("Fail to create a socket.");
    }
    ///detail of socket
    bzero(&MainServerInfo,sizeof(MainServerInfo));
    MainServerInfo.sin_family = PF_INET;
    MainServerInfo.sin_addr.s_addr = INADDR_ANY;
    MainServerInfo.sin_port = htons(59487);
    bind(MainServerSocket,(struct sockaddr *)&MainServerInfo,sizeof(MainServerInfo));
    listen(MainServerSocket,5);

    while(1)
    {
        LogFiles=fopen("Log.txt","a");
        GetTime();
        printf("S-Cache controller stand by...\n");
        fprintf(LogFiles,"S-Cache controller stand by...\n");
        fclose(LogFiles);
        char inputBuffer[256]={};
        char message[]={"Finished"};
        GetClientSocket=accept(MainServerSocket,(struct sockaddr*)&MainClientInfo,&addrlen);

        recv(GetClientSocket,inputBuffer,sizeof(inputBuffer),0);
        LogFiles=fopen("Log.txt","a");
        GetTime();
        printf("Server get: %s\n",inputBuffer);
        fprintf(LogFiles,"Server get: %s\n",inputBuffer);
        RequestCounter++;
        ///increase timer
        int TimeForJob;
        for(TimeForJob=0;TimeForJob<ServiceCount;TimeForJob++)
        {
            if(CacheStatus[TimeForJob]==1)
            {
                JobTimer[TimeForJob]++;
            }
        }
        FlagForSecondTime=0;
        ///check which service
        if(strcmp(inputBuffer,"CirrOS")==0)
        {
            if(CacheStatus[CirrOS]==0)
            {
                if(TimingToClone(CirrOS))
                {
                    GetTime();
                    printf("Clone CirrOS to edge cloud\n");
                    fprintf(LogFiles,"Clone CirrOS to edge cloud\n");
                    if(Policy(CirrOS))
                    {
                        CacheStatus[CirrOS]=1;
                        ImageStatus[CirrOS]=1;
                        JobHitEdge[CirrOS]++;
                        ImageHit[CirrOS]++;
                    }
                    else
                    {
                        GetTime();
                        printf("Error and relaunch\n");
                        fprintf(LogFiles,"Error and relaunch\n");
                        FlagForSecondTime=1;
                        TimingToClone(CirrOS);
                    }
                }
                else
                {
                    GetTime();
                    printf("Not clone CirrOS to edge cloud\n");
                    fprintf(LogFiles,"Not clone CirrOS to edge cloud\n");
                }
            }
            else
            {
                GetTime();
                JobHitEdge[CirrOS]++;
                fprintf(LogFiles,"Hit CirrOS\n");
                printf("Hit CirrOS\n");
                HitCounter++;
            }
        }
        else if(strcmp(inputBuffer,"WebServer")==0)
        {
            if(CacheStatus[WebServer]==0)
            {
                if(TimingToClone(WebServer))
                {
                    GetTime();
                    printf("Clone WebServer to edge cloud\n");
                    fprintf(LogFiles,"Clone WebServer to edge cloud\n");
                    if(Policy(WebServer))
                    {
                        CacheStatus[WebServer]=1;
                        ImageStatus[WebServer]=1;
                        JobHitEdge[WebServer]++;
                        ImageHit[WebServer]++;
                    }
                    else
                    {
                        GetTime();
                        fprintf(LogFiles,"Error and relaunch\n");
                        printf("Error and relaunch\n");
                        FlagForSecondTime=1;
                        TimingToClone(WebServer);
                    }
                }
                else
                {
                    GetTime();
                    printf("Not clone WebServer to edge cloud\n");
                    fprintf(LogFiles,"Not clone WebServer to edge cloud\n");
                }
            }
            else
            {
                JobHitEdge[WebServer]++;
                GetTime();
                fprintf(LogFiles,"Hit WebServer\n");
                printf("Hit WebServer\n");
                HitCounter++;

            }
        }
        else if(strcmp(inputBuffer,"FtpServer")==0)
        {
            if(CacheStatus[FtpServer]==0)
            {
                if(TimingToClone(FtpServer))
                {
                    GetTime();
                    printf("Clone FtpServer to edge cloud\n");
                    fprintf(LogFiles,"Clone FtpServer to edge cloud\n");
                    if(Policy(FtpServer))
                    {
                        CacheStatus[FtpServer]=1;
                        ImageStatus[FtpServer]=1;
                        JobHitEdge[FtpServer]++;
                        ImageHit[FtpServer]++;
                    }
                    else
                    {
                        GetTime();
                        fprintf(LogFiles,"Error and relaunch\n");
                        printf("Error and relaunch\n");
                        FlagForSecondTime=1;
                        TimingToClone(FtpServer);
                    }
                }
                else
                {
                    GetTime();
                    printf("Not clone FtpServer to edge cloud\n");
                    fprintf(LogFiles,"Not clone FtpServer to edge cloud\n");
                }
            }
            else
            {
                GetTime();
                JobHitEdge[FtpServer]++;
                fprintf(LogFiles,"Hit FtpServer\n");
                printf("Hit FtpServer\n");
                HitCounter++;
            }
        }
        else if(strcmp(inputBuffer,"CentOS7")==0)
        {
            if(CacheStatus[CentOS7]==0)
            {
                if(TimingToClone(CentOS7))
                {
                    GetTime();
                    printf("Clone CentOS7 to edge cloud\n");
                    fprintf(LogFiles,"Clone CentOS7 to edge cloud\n");
                    if(Policy(CentOS7))
                    {
                        CacheStatus[CentOS7]=1;
                        ImageStatus[CentOS7]=1;
                        JobHitEdge[CentOS7]++;
                        ImageHit[CentOS7]++;
                    }
                    else
                    {
                        GetTime();
                        fprintf(LogFiles,"Error and relaunch\n");
                        printf("Error and relaunch\n");
                        FlagForSecondTime=1;
                        TimingToClone(CentOS7);
                    }
                }
                else
                {
                    GetTime();
                    printf("Not clone FtpServer to edge cloud\n");
                    fprintf(LogFiles,"Not clone FtpServer to edge cloud\n");
                }
            }
            else
            {
                GetTime();
                JobHitEdge[CentOS7]++;
                fprintf(LogFiles,"Hit CentOS7\n");
                printf("Hit CentOS7\n");
                HitCounter++;
            }
        }
        PrintStatus();
        send(GetClientSocket,message,sizeof(message),0);
    }
}
void PrintStatus()
{
        printf("----------------------------------------------\n");
        GetTime();
        fprintf(LogFiles,"Total hit: %.0f / Total request: %.0f / Hit rate: %.2f\n",HitCounter,RequestCounter,HitCounter/RequestCounter);
        printf("Total hit: %.0f / Total request: %.0f / Hit rate: %.2f\n",HitCounter,RequestCounter,HitCounter/RequestCounter);
        GetTime();
        printf("CirrOS hit : %.0f Instance Cache: %d Image Cache; %d\n",JobHitPast[CirrOS]+JobHitEdge[CirrOS],CacheStatus[CirrOS],ImageStatus[CirrOS]);
        fprintf(LogFiles,"CirrOS hit : %.0f Instance Cache: %d Image Cache; %d\n",JobHitPast[CirrOS]+JobHitEdge[CirrOS],CacheStatus[CirrOS],ImageStatus[CirrOS]);
        GetTime();
        printf("WebServer hit : %.0f Instance Cache: %d Image Cache; %d\n",JobHitPast[WebServer]+JobHitEdge[WebServer],CacheStatus[WebServer],ImageStatus[WebServer]);
        fprintf(LogFiles,"WebServer hit : %.0f Instance Cache: %d Image Cache; %d\n",JobHitPast[WebServer]+JobHitEdge[WebServer],CacheStatus[WebServer],ImageStatus[WebServer]);
        GetTime();
        printf("FtpServer hit : %.0f Instance Cache: %d Image Cache; %d\n",JobHitPast[FtpServer]+JobHitEdge[FtpServer],CacheStatus[FtpServer],ImageStatus[FtpServer]);
        fprintf(LogFiles,"FtpServer hit : %.0f Instance Cache: %d Image Cache; %d\n",JobHitPast[FtpServer]+JobHitEdge[FtpServer],CacheStatus[FtpServer],ImageStatus[FtpServer]);
        GetTime();
        printf("CentOS7 hit : %.0f Instance Cache: %d Image Cache; %d\n",JobHitPast[CentOS7]+JobHitEdge[CentOS7],CacheStatus[CentOS7],ImageStatus[CentOS7]);
        fprintf(LogFiles,"CentOS7 hit : %.0f Instance Cache: %d Image Cache; %d\n",JobHitPast[CentOS7]+JobHitEdge[CentOS7],CacheStatus[CentOS7],ImageStatus[CentOS7]);
        printf("----------------------------------------------\n");
        fprintf(LogFiles,"----------------------------------------------\n");
        printf("----------------------------------------------\n");
        fclose(LogFiles);
}
///Check the edge cloud resources
void CheckEdgeResources()
{
    GetTime();
    fprintf(LogFiles,"Check the resouces of edge cloud...\n");
    printf("Check the resouces of edge cloud...\n");
    ///write the command and get resources
    FILE *ResourceReturn;
    char ResoucesBuffer[500]={};
    ResourceReturn = popen(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack host show edge","r");

    ///splitte the string and find the resource volume
    int FindVolume,BlockCount=0;
    char *Splitter="|";
    char *pointer;
    char *CutForResources[100];
    for(FindVolume=0;FindVolume<6;FindVolume++)
    {
        fgets(ResoucesBuffer,500,ResourceReturn);
        if(FindVolume==5)
        {
            pointer=strtok(ResoucesBuffer,Splitter);
            while(pointer!=NULL)
            {
                CutForResources[BlockCount]=pointer;
                pointer=strtok(NULL,Splitter);
                BlockCount++;
            }
        }
    }
    CurrentCpu=atoi(CutForResources[2]);
    CurrentRam=atoi(CutForResources[3]);
    CurrentDisk=atoi(CutForResources[4]);
    GetTime();
    fprintf(LogFiles,"Current resouces: CPU: %d RAM: %d Disk: %d ImageDisk %d\n",atoi(CutForResources[2]),atoi(CutForResources[3]),atoi(CutForResources[4]),CurrentImageDisk);
    printf("Current resouces: CPU: %d RAM: %d Disk: %d ImageDisk %d\n",atoi(CutForResources[2]),atoi(CutForResources[3]),atoi(CutForResources[4]),CurrentImageDisk);
}
///Launch service
int LaunchInstance(int WhichService)
{

    DeleteFloatIP(WhichService);
    FILE *LaunchReturn;
    char Feedback[500];
    if(WhichService==0)
    {
        LaunchReturn = popen(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server create --flavor m1.tiny --image CirrOS --nic net-id=PrivateNet --key-name key --availability-zone nova:edge CirrOS_Edge","r");
    }
    if(WhichService==1)
    {
        LaunchReturn = popen(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server create --flavor m1.small --image Ubuntu_1604_CML_WebServer --nic net-id=PrivateNet --key-name key --availability-zone nova:edge WebServer_Edge","r");
    }
    if(WhichService==2)
    {
        LaunchReturn = popen(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server create --flavor m1.small --image CentOS_7_CML_FtpServer --nic net-id=PrivateNet --key-name key --availability-zone nova:edge FtpServer_Edge","r");
    }
    if(WhichService==3)
    {
        LaunchReturn = popen(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server create --flavor m1.medium --image CentOS_7_GUI --nic net-id=PrivateNet --key-name key --availability-zone nova:edge CentOS7_Edge","r");
    }
    fgets(Feedback,500,LaunchReturn);
    GetTime();
    printf("Launch service command has been processed\n");
    fprintf(LogFiles,"Launch service command has been processed\n");
    sleep(5);
    if(CheckInstanceStatus())
    {
        if(ImageStatus[WhichService]==0)
        {
            CurrentImageDisk+=ServiceResources[WhichService][2];
        }
        AllocateFloatIPtoEdge(WhichService);
        return 1;
    }
    else return 0;


}
///Check service status
int CheckInstanceStatus()
{
    GetTime();
    fprintf(LogFiles,"Service status checking...\n");
    printf("Service status checking...\n");
    while(1)
    {
        FILE *StatusReturn;
        char EdgeStatus[500]={};
        StatusReturn = popen(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server list --host edge","r");

        ///splitte the string and return service
        int CountCacheNumber=0;
        int BlockCount=0;
        char *Splitter="|";
        char *pointer;
        char *ServiceStatus[100];
        while(1)
        {
            fgets(EdgeStatus,500,StatusReturn);
            CountCacheNumber++;
            if(CountCacheNumber==4)
            {
                pointer=strtok(EdgeStatus,Splitter);
                while(pointer!=NULL)
                {
                    ServiceStatus[BlockCount]=pointer;
                    pointer=strtok(NULL,Splitter);
                    BlockCount++;
                }
                GetTime();
                fprintf(LogFiles,"%s %s %s\n",ServiceStatus[0],ServiceStatus[1],ServiceStatus[2]);
                printf("%s %s %s\n",ServiceStatus[0],ServiceStatus[1],ServiceStatus[2]);
                break;
            }
        }
        if(strcmp(ServiceStatus[2]," ACTIVE ")==0)
        {
            break;
        }
        if(strcmp(ServiceStatus[2]," ERROR ")==0)
        {
            char DeleteErrorInstance[500];
            strcpy(DeleteErrorInstance,". /home/ccnlab/Desktop/admin-openrc.sh \n nova delete");
            strcat(DeleteErrorInstance,ServiceStatus[1]);
            system(DeleteErrorInstance);
            return 0;
        }
    }
    return 1;
}
///The timing to clone service
int TimingToClone(int WhichService)
{
    if(JobHitPast[WhichService]==0 || FlagForSecondTime==1)
    {
        CheckEdgeResources();
        return 1;
    }
    else
    {
        int Index=0;
        int mIndex=0;
        int TempCacheStatus[ServiceCount];
        int TempCpu,TempRam,TempDisk;
        double mScore=0;
        double CacheScore=0;
        double NewServiceScore=0;

        CheckEdgeResources();
        TempCpu=CurrentCpu;
        TempRam=CurrentRam;
        TempDisk=CurrentDisk;
        for(Index=0;Index<ServiceCount;Index++)
        {
            TempCacheStatus[Index]=CacheStatus[Index];
        }

        while((ServiceResources[WhichService][0]+TempCpu)>QuotaCpu || (ServiceResources[WhichService][1]+TempRam)>QuotaRam || (ServiceResources[WhichService][2]+TempDisk)>QuotaDisk)
        {
            ///CPU
            if(ServiceResources[WhichService][0]+TempCpu>QuotaCpu)
            {
                for(Index=0;Index<ServiceCount;Index++)
                {
                    if(TempCacheStatus[Index]==1)
                    {
                        CacheScore=(JobHitEdge[Index]/(JobTimer[Index]*((7*NorServiceResources[Index][0])+NorServiceResources[Index][1]+NorServiceResources[Index][2])));
                        if(CacheScore<mScore || mScore==0)
                        {
                            mScore=CacheScore;
                            mIndex=Index;
                        }
                    }
                }
                TempCacheStatus[mIndex]=0;
                NewServiceScore=(JobHitPast[WhichService]/(1*((7*NorServiceResources[WhichService][0])+NorServiceResources[WhichService][1]+NorServiceResources[WhichService][2])));
            }
            ///RAM
            else if(ServiceResources[WhichService][1]+CurrentRam>QuotaRam)
            {
                for(Index=0;Index<ServiceCount;Index++)
                {
                    if(TempCacheStatus[Index]==1)
                    {
                        CacheScore=(JobHitEdge[Index]/(JobTimer[Index]*(NorServiceResources[Index][0]+(7*NorServiceResources[Index][1])+NorServiceResources[Index][2])));
                        if(CacheScore<mScore || mScore==0)
                        {
                            mScore=CacheScore;
                            mIndex=Index;
                        }
                    }
                }
                TempCacheStatus[mIndex]=0;
                NewServiceScore=(JobHitPast[WhichService]/(1*(NorServiceResources[WhichService][0]+(7*NorServiceResources[WhichService][1])+NorServiceResources[WhichService][2])));
            }
            ///Disk
            else if(ServiceResources[WhichService][2]+CurrentDisk>QuotaDisk)
            {
                for(Index=0;Index<ServiceCount;Index++)
                {
                    if(TempCacheStatus[Index]==1)
                    {
                        CacheScore=(JobHitEdge[Index]/(JobTimer[Index]*(NorServiceResources[Index][0]+NorServiceResources[Index][1]+(7*NorServiceResources[Index][2]))));
                        if(CacheScore<mScore || mScore==0)
                        {
                            mScore=CacheScore;
                            mIndex=Index;
                        }
                    }
                }
                TempCacheStatus[mIndex]=0;
                NewServiceScore=(JobHitPast[WhichService]/(1*(NorServiceResources[WhichService][0]+NorServiceResources[WhichService][1]+(7*NorServiceResources[WhichService][2]))));
            }
            if(NewServiceScore>mScore)
            {
                TempCpu=TempCpu-ServiceResources[mIndex][0];
                TempRam=TempRam-ServiceResources[mIndex][1];
                TempDisk=TempDisk-ServiceResources[mIndex][2];
            }
            else
            {
                return 0;
                break;
            }
        }
        return 1;
    }
}
///S-Cache Policy
int Policy(int WhichService)
{
    while(1)
    {
        GetTime();
        fprintf(LogFiles,"TempResouces: CPU: %d RAM: %d Disk: %d ImageDisk: %d\n",ServiceResources[WhichService][0]+CurrentCpu,ServiceResources[WhichService][1]+CurrentRam,ServiceResources[WhichService][2]+CurrentDisk,ServiceResources[WhichService][2]+CurrentImageDisk);
        printf("TempResouces: CPU: %d RAM: %d Disk: %d ImageDisk: %d\n",ServiceResources[WhichService][0]+CurrentCpu,ServiceResources[WhichService][1]+CurrentRam,ServiceResources[WhichService][2]+CurrentDisk,ServiceResources[WhichService][2]+CurrentImageDisk);
        if((ServiceResources[WhichService][0]+CurrentCpu)>QuotaCpu || (ServiceResources[WhichService][1]+CurrentRam)>QuotaRam || (ServiceResources[WhichService][2]+CurrentDisk)>QuotaDisk || FlagForSecondTime==1 || ServiceResources[WhichService][2]+CurrentImageDisk>QuotaDisk)
        {
            int Index=0;
            int mIndex=0;
            double mScore=0;
            double CacheScore=0;
            ///CPU
            if(ServiceResources[WhichService][0]+CurrentCpu>QuotaCpu)
            {
                GetTime();
                printf("%sCPU is not enough\n",KeyRed);
                printf("%s",ResetColor);
                fprintf(LogFiles,"CPU is not enough\n");
                for(Index=0;Index<ServiceCount;Index++)
                {
                    if(CacheStatus[Index]==1)
                    {
                        CacheScore=(JobHitEdge[Index]/(JobTimer[Index]*((7*NorServiceResources[Index][0])+NorServiceResources[Index][1]+NorServiceResources[Index][2])));
                        if(CacheScore<mScore || mScore==0)
                        {
                            mScore=CacheScore;
                            mIndex=Index;
                        }
                    }
                }
                DeleteVM(mIndex);
            }
            ///RAM
            else if(ServiceResources[WhichService][1]+CurrentRam>QuotaRam)
            {
                GetTime();
                fprintf(LogFiles,"RAM is not enough\n");
                printf("%sRam is not enough\n",KeyRed);
                printf("%s",ResetColor);
                for(Index=0;Index<ServiceCount;Index++)
                {
                    if(CacheStatus[Index]==1)
                    {
                        CacheScore=(JobHitEdge[Index]/(JobTimer[Index]*(NorServiceResources[Index][0]+(7*NorServiceResources[Index][1])+NorServiceResources[Index][2])));
                        if(CacheScore<mScore || mScore==0)
                        {
                            mScore=CacheScore;
                            mIndex=Index;
                        }
                    }
                }
                DeleteVM(mIndex);
            }
            ///Disk
            else if(ServiceResources[WhichService][2]+CurrentDisk>QuotaDisk || ServiceResources[WhichService][2]+CurrentImageDisk>QuotaDisk)
            {
                int StoringImage=0;
                int mImageIndex=0;
                double mImageScore=0;
                double ImageScore=0;
                if(ServiceResources[WhichService][2]+CurrentImageDisk>QuotaDisk)
                {
                    for(Index=0;Index<ServiceCount;Index++)
                    {
                        if(ImageStatus[Index]==1 && CacheStatus[Index]==0)
                        {
                            StoringImage=1;
                            ImageScore=Clock+(ImageHit[Index]*(1/ImageResources[Index]));
                            if(ImageScore<mImageScore || mImageScore==0)
                            {
                                mImageScore=ImageScore;
                                mImageIndex=Index;
                            }
                        }
                    }
                }
                if(StoringImage==1)
                {
                    GetTime();
                    fprintf(LogFiles,"Image Disk is not enough\n");
                    printf("%sImage Disk is not enough\n",KeyRed);
                    printf("%s",ResetColor);
                    Clock=mImageScore;
                    DeleteImage(mImageIndex);
                    CurrentImageDisk-=ServiceResources[mImageIndex][2];
                }
                if(StoringImage==0)
                {
                    GetTime();
                    fprintf(LogFiles,"Disk is not enough\n");
                    printf("%sDisk is not enough\n",KeyRed);
                    printf("%s",ResetColor);
                    for(Index=0;Index<ServiceCount;Index++)
                    {
                        if(CacheStatus[Index]==1)
                        {
                            CacheScore=(JobHitEdge[Index]/(JobTimer[Index]*(NorServiceResources[Index][0]+NorServiceResources[Index][1]+NorServiceResources[Index][2])));
                            if(CacheScore<mScore || mScore==0)
                            {
                                mScore=CacheScore;
                                mIndex=Index;
                            }
                        }
                    }
                    DeleteVM(mIndex);
                }
            }
            ///Second Time
            else
            {
                for(Index=0;Index<ServiceCount;Index++)
                {
                    if(CacheStatus[Index]==1)
                    {
                        CacheScore=(JobHitEdge[Index]/(JobTimer[Index]*(NorServiceResources[Index][0]+NorServiceResources[Index][1]+NorServiceResources[Index][2])));
                        if(CacheScore<mScore || mScore==0)
                        {
                            mScore=CacheScore;
                            mIndex=Index;
                        }
                    }
                }
                DeleteVM(mIndex);
            }
        }
        else break;
    }
    return LaunchInstance(WhichService);
}
///Delete VM instance
void DeleteVM(int mIndex)
{
    FILE *DeleteReturn;
    char Feedback[500];
    if(mIndex==0)
    {
        DeleteReturn = popen(". /home/ccnlab/Desktop/admin-openrc.sh \n nova delete CirrOS_Edge","r");
        GetTime();
        printf("%sDelete CirrOS\n",KeyRed);
        printf("%s",ResetColor);
        fprintf(LogFiles,"Delete CirrOS\n");
    }
    if(mIndex==1)
    {
        DeleteReturn = popen(". /home/ccnlab/Desktop/admin-openrc.sh \n nova delete WebServer_Edge","r");
        GetTime();
        printf("%sDelete WebServer\n",KeyRed);
        printf("%s",ResetColor);
        fprintf(LogFiles,"Delete WebServer\n");
    }
    if(mIndex==2)
    {
        DeleteReturn = popen(". /home/ccnlab/Desktop/admin-openrc.sh \n nova delete FtpServer_Edge","r");
        GetTime();
        printf("%sDelete FtpServer\n",KeyRed);
        printf("%s",ResetColor);
        fprintf(LogFiles,"Delete FtpServer\n");
    }
    if(mIndex==3)
    {
        DeleteReturn = popen(". /home/ccnlab/Desktop/admin-openrc.sh \n nova delete CentOS7_Edge","r");
        GetTime();
        printf("%sDelete CentOS7\n",KeyRed);
        printf("%s",ResetColor);
        fprintf(LogFiles,"Delete CentOS7\n");
    }
    fgets(Feedback,500,DeleteReturn);
    sleep(5);
    int tempCpu=CurrentCpu;
    CheckEdgeResources();
    if(CurrentCpu!=tempCpu)
    {
        AllocateFloatIPtoMajor(mIndex);
        CacheStatus[mIndex]=0;
        JobHitPast[mIndex]=JobHitEdge[mIndex];
        JobHitEdge[mIndex]=0;
        JobTimer[mIndex]=0;
    }
    else DeleteVM(mIndex);
}
///Delete Image
void DeleteImage(int mIndex)
{
    ///Create Socket
    int DeleteImageClient=0;
    DeleteImageClient=socket(AF_INET,SOCK_STREAM,0);
    if(DeleteImageClient ==-1)
    {
        printf("Fail to create a socket.");
    }
    ///Detail of socket
    struct sockaddr_in DeleteImageInfo;
    bzero(&DeleteImageInfo,sizeof(DeleteImageInfo));
    DeleteImageInfo.sin_family = PF_INET;
    DeleteImageInfo.sin_addr.s_addr = inet_addr("192.168.168.13");
    DeleteImageInfo.sin_port = htons(9487);

    int err=connect(DeleteImageClient,(struct sockaddr *)&DeleteImageInfo,sizeof(DeleteImageInfo));
    if(err==-1)
    {
        printf("Connection error");
    }
    ///send which iamge should be deleted.
    char receiveMessage[100] = {};
    char sendMessage[100];
    sprintf(sendMessage,"%d",mIndex);
    send(DeleteImageClient,sendMessage,sizeof(sendMessage),0);
    recv(DeleteImageClient,receiveMessage,sizeof(receiveMessage),0);
    ImageStatus[mIndex]=0;
    GetTime();
    fprintf(LogFiles,"%s\n",receiveMessage);
    printf("%s\n",receiveMessage);
    close(DeleteImageClient);
}
///Allocate float IP to edge services
void AllocateFloatIPtoEdge(int WhichService)
{
    GetTime();
    fprintf(LogFiles,"Allocate float IP to edge cloud service\n");
    printf("Allocate float IP to edge cloud service\n");
    if(WhichService==0)
    {
        system(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server add floating ip CirrOS_Edge 192.168.168.170");
    }
    if(WhichService==1)
    {
        system(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server add floating ip WebServer_Edge 192.168.168.173");
    }
    if(WhichService==2)
    {
        system(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server add floating ip FtpServer_Edge 192.168.168.174");
    }
    if(WhichService==3)
    {
        system(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server add floating ip CentOS7_Edge 192.168.168.176");
    }
}
///Allocate float IP back to Major service
void AllocateFloatIPtoMajor(int WhichService)
{
    GetTime();
    fprintf(LogFiles,"Allocate float IP back to Major service\n");
    printf("Allocate float IP back to Major service\n");
    if(WhichService==0)
    {
        system(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server add floating ip CirrOS_Major 192.168.168.170");
    }
    if(WhichService==1)
    {
        system(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server add floating ip WebServer_Major 192.168.168.173");
    }
    if(WhichService==2)
    {
        system(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server add floating ip FtpServer_Major 192.168.168.174");
    }
    if(WhichService==3)
    {
        system(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server add floating ip CentOS7_Major 192.168.168.176");
    }
}
///Delete float IP from major services
void DeleteFloatIP(int WhichService)
{
    GetTime();
    fprintf(LogFiles,"Delete float IP from major cloud service\n");
    printf("Delete float IP from major cloud service\n");
    if(WhichService==0)
    {
        system(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server remove floating ip CirrOS_Major 192.168.168.170");
    }
    if(WhichService==1)
    {
        system(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server remove floating ip WebServer_Major 192.168.168.173");
    }
    if(WhichService==2)
    {
        system(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server remove floating ip FtpServer_Major 192.168.168.174");
    }
    if(WhichService==3)
    {
        system(". /home/ccnlab/Desktop/admin-openrc.sh \n openstack server remove floating ip CentOS7_Major 192.168.168.176");
    }
}
///Get current time
void GetTime()
{
    time_t now=time(0);
    PrintfTime=localtime(&now);
    strftime(TimeBuffer,sizeof(TimeBuffer),"%Y-%m-%d %H:%M:%S", PrintfTime);
    printf ("%s ",TimeBuffer);
    fprintf(LogFiles,"%s ",TimeBuffer);
}
