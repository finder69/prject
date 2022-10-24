#include <iostream>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <unistd.h>


#include <pthread.h>
#include <arpa/inet.h>

using namespace std;

// 定义一个结构体 SockInfo 与struct不同的是使用结构体时不需要再用struct关键字  直接SockInfo 对象
typedef struct SockInfo{
    int fd;
    struct sockaddr_in addr;
    socklen_t addr_len;
    pthread_t id;
}SockInfo;

void* 
pthread_Callback(void* arg);

#define server_port 9527

int main(int argc, char *argv[])                                                         
{

    // 创建套接字
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_port = htons(server_port);
    serv_addr.sin_family = AF_INET;
    socklen_t serv_len = sizeof(serv_addr);
    int ret;
    if((ret =bind(lfd,(struct sockaddr*)&serv_addr,serv_len)) != 0){
        perror("err: bind");
        exit(1);
    }
    if((ret =listen(lfd,36))!=0)
    {
        perror("err: listen");
        exit(2);
    }

    printf("Start accept\n");


    // 设置线程属性所有的
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // 设置的线程之间是独立的
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

    SockInfo info[256];
    for (int j=0;j<sizeof(info)/sizeof(SockInfo);++j)
        info[j].fd = -1;

    int i;
    int p =-1;
    // cout<<"大小： "<<sizeof(info)/sizeof(SockInfo)<<endl;  //256
    while(1)
    {
        ++p;
        cout<<"当前p： "<<p<<endl;
        // 如果info[i]== -1表示accpet函数与客户端连接失败 如果info[i]为其他值表示连接成功
        for (i=0;i<sizeof(info)/sizeof(SockInfo);++i)
        {
            if(info[i].fd == -1)   
                break;
        }
        if(i == 256)
            break;

        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);

        // 主线程等待接收连接请求
        info[i].fd =accept(lfd,(struct sockaddr*)&cli_addr,&cli_len); 

        info[i].addr = cli_addr;
        info[i].addr_len = cli_len;


        // 创建子线程通讯
        pthread_t tid;
        // 第一个参数是线程标识符，第二个是线程属性（要指针），第三个是线程要执行的函数，第四个是传入执行函数的实参(规范传入需要转换成void*类型)
        pthread_create(&tid,&attr,pthread_Callback,(void *)&info[i]);
        cout<<"当前i = "<<i<<endl;
        cout<<"当前进程id: "<<info[i].id<<endl;;
        cout<<"当前fd: "<<info[i].fd<<endl;
    }
    close(lfd);
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
    return 0;
}

void* 
pthread_Callback(void* arg){
    SockInfo *info = (SockInfo*)arg;
    while(1)
    {
        char ip[64]={0};                                                                 

        char buf[1024];
        printf("New Client IP: %s, Port: %d\n",
               inet_ntop(AF_INET,&info->addr.sin_addr.s_addr,ip,sizeof(ip)),
               ntohs(info->addr.sin_port));
        int len = read(info->fd,buf,sizeof(buf));
        if(len == -1)
        {
            perror("read err");
            pthread_exit(NULL);
        }else if(len == 0)
        {
            printf("客户端断开连接\n");
            close(info->fd);
            break;
        }else{
            printf("recv: %s\n",buf);
            write(info->fd,buf,len);
        }
    }
    pthread_exit(NULL);
}
