#ifdef  __MYSOCKET_H__
#define __MYSOCKET_H__

typedef void HANDLE;

int opensocket(HANDLE **);//打开socket
int setsocket(HANDLE *);//设置socket
int connectsocket(HANDLE *);//连接socket
int closesocket(HANDLE *);//关闭socket
int senddata(HANDLE *, char *buf, int nbyte);//发送数据
int recvdata(HANDLE *, char *buf, int nbyte);//接收数据

#endif /* __MYSOCKET_H__ */