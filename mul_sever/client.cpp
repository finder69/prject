#include <iostream>

#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;
#define ServerPort 9527
int main(int argc, char *argv[])                                                         
{
    //创建套接字
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    
    // 设置服务端地址结构
    struct sockaddr_in serv;
    bzero(&serv,sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(ServerPort);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    // inet_pton 作用：将ip地址从字符串转换成为网络地址格式
    inet_pton(AF_INET,"127.0.0.1",&(serv.sin_addr.s_addr));
    // 连接服务器
    if(connect(lfd,(struct sockaddr*)&serv,sizeof(serv)))
    {
        printf("Connect erro");
        exit(1);
    }

    char buf[1024] = {0};
    while(1)
    {

        // fgets函数相当于cin输入，从stdin的流中读取一行，并把它存储在buf所指向的字符串内，有换行符的时候或者超过大小会停止
        fgets(buf,sizeof(buf),stdin);
        send(lfd,buf,sizeof(buf),0);
        recv(lfd,buf,sizeof(buf),0);
        cout<<buf<<endl;
    }
    return 0;
}

