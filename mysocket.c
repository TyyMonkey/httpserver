#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "mysocket.h"

typedef struct _TcpSocket
{
	int fd;
}TcpSocket, *TcpSocketPtr;

//打开socket
int opensocket(void **handle)
{
	TcpSocketPtr sock = NULL;
	
	//创建socket
	int fd = socket(AF_INET, SOCK_STREAM, 0);	
	
	if(fd == -1)
	{
		printf("error : %s\n", strerror(errno));
		return -1;
	}
	
	sock = (TcpSocketPtr)malloc(sizeof(TcpSocket));
	memset(sock, 0, sizeof(TcpSocket));
	sock->fd = fd;
	
	*handle = (void *)sock;
	
	return 0;
}

int setsocket(void *handle)//设置socket
{
	return 0;
}

int connectsocket(void *handle, char *serverIP, int port)//连接socket
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(struct sockaddr_in));	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(serverIP);
	int ret = connect( ((TcpSocketPtr)handle)->fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
	if(ret == -1)
	{
		printf("connect error : %s\n", strerror(errno));
		return -1;
	}
	
	return 0;
}

int closesocket(void *handle)//关闭socket
{
	//关闭socket
	return close( ((TcpSocketPtr)handle)->fd );
}

int senddata(void *handle, char *buf, int nbyte)//发送数据
{
	//发送数据或接收数据
	int fd = ((TcpSocketPtr)handle)->fd;
	
	int ret = write(fd, buf, nbyte);	
	
	return ret;
}
int recvdata(void *handle, char *buf, int nbyte)//接收数据
{
	//接收数据
	int fd = ((TcpSocketPtr)handle)->fd;
	
	return recv(fd, buf, nbyte, 0);
}

int startClient(char *serverIP, int port)
{
	void *handle;
	//1. 创建socket
	if(opensocket(&handle) == -1)
	{
		return -1;
	}
	
	//2. connect服务端
	if(connectsocket(handle, serverIP, port) == -1)
	{
		return -1;
	}
	
	//3. 发送数据或接收数据
	char buf[1024] = {0};
	strcpy(buf, "Hello world!\n");
  if ( senddata(handle, buf, strlen(buf)) >0 )
  {
  	
  }
	
	//4. 关闭socket
	closesocket(handle);
}

void startServer(int port)
{
	int ret = 0;
	//1. 创建socket
	int fdsocket = socket(AF_INET, SOCK_STREAM, 0);	
	
	if(fdsocket == -1)
	{
		printf("socket error : %s\n", strerror(errno));
		return;
	}
	
	//2.  bind绑定
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
  ret = bind(fdsocket, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
  if(ret == -1)
  {
  	printf("bind error : %s", strerror(errno));
  	return;
  }

	//3. listen
	ret = listen(fdsocket, 10);	
  if(ret == -1)
  {
  	printf("listen error : %s", strerror(errno));
  	return;
  }
  
	//4. accept 等待连接
	struct sockaddr_in clientAddr;
	socklen_t addrlen;
	ret = accept(fdsocket, (struct sockaddr *)&clientAddr, &addrlen);
  if(ret == -1)
  {
  	printf("accept error : %s", strerror(errno));
  	return;
  }
  
	//5. recv
	char buf[1024] = {0};
	ret = recv(fdsocket, buf, sizeof(buf), 0);
	
	if(ret > 0)
	{
		printf("server receive : %s\n", buf);
	}else 
	if(ret == 0)
	{
		printf("client close success!\n");
	}else 
	{
		printf(" recv error : %s\n", strerror(errno));
	}
	
	close(fdsocket);
}


int main(int argc, char *argv[])
{
	
	if(argc < 2)
		return 0;
		
	if(atoi(argv[1]) == 1) //client
	{
		printf("启动 client\n");
		
		startClient("192.168.77.253", 22222);		
	}else 
	if(atoi(argv[1]) == 2) //server
	{
		printf("启动 server\n");
		startServer(22222);
	}
   
  return 0;
}
