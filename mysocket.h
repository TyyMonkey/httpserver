#ifdef  __MYSOCKET_H__
#define __MYSOCKET_H__

typedef void HANDLE;

int opensocket(HANDLE **);//��socket
int setsocket(HANDLE *);//����socket
int connectsocket(HANDLE *);//����socket
int closesocket(HANDLE *);//�ر�socket
int senddata(HANDLE *, char *buf, int nbyte);//��������
int recvdata(HANDLE *, char *buf, int nbyte);//��������

#endif /* __MYSOCKET_H__ */