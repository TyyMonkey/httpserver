#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "pub.h"

#define BACKLOG_MAX 256

int socket_server_create(int port)
{
	int stfd;
	//创建tcp
	stfd = socket(AF_INET, SOCK_STREAM, 0);
	if(stfd < 0)
	{
		printf("socket failed %s\n", strerror(errno));
		return -1;
	}
	
	int opt = 1;
	socklen_t optlen = sizeof(opt);
  setsockopt(stfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&opt, optlen);

	//bind
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	//#define    INADDR_ANY        ((in_addr_t) 0x00000000)
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(stfd, (const struct sockaddr *)&addr, len) < 0)
	{
		printf("bind failed %s\n", strerror(errno));
		return -1;
	}
	
	//listen
	//int listen(int sockfd, int backlog);
	if(listen(stfd, BACKLOG_MAX) < 0)
	{
		printf("listen failed %s\n", strerror(errno));
		return -1;		
	}
	
	return stfd;
}

int socket_client_create()
{
	int stfd;
	//创建tcp
	stfd = socket(AF_INET, SOCK_STREAM, 0);
	if(stfd < 0)
	{
		printf("socket failed %s\n", strerror(errno));
		return -1;
	}
	
	return stfd;	
}

int socket_accept(int fd)
{
	//int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	int clientfd;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	memset(&addr, 0, sizeof(addr));
	if( (clientfd = accept(fd, (struct sockaddr *)&addr, &len)) < 0)
	{
		printf("accept failed %s \n", strerror(errno));
	}else
	{
		printf("accept by %s \n", inet_ntoa(addr.sin_addr));
	}
	
	return clientfd;
}

int socket_connect(int fd, const char *ip, int port)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	if(connect(fd, (const struct sockaddr *)&addr, len) < 0)
	{
		printf("connect failed %s\n", strerror(errno));
		return -1;
	}else
	{
		printf("connect %s success \n", ip);
		return 0;
	}
}
int setnonblock(int fd)
{
	int opts = fcntl(fd, F_GETFL, 0);
	
	if(opts < 0)
	{
		printf("fcntl get failed %s\n", strerror(errno));
		return -1;
	}else
	{
		if (fcntl(fd, F_SETFL, opts | O_NONBLOCK)  < 0)
		{
			printf("fcntl set failed %s\n", strerror(errno));
			return -1;
		}
	}
	return 0;
}

int readfile(const char *filename, char **content)
{
	int ret = 0;
	struct stat buf;
	if(NULL == filename)
	{
		ret = -1;
		goto end;
	}
	FILE *fp = NULL;
	fp = fopen(filename, "rb");
	if(fp == NULL)
	{
		ret = -1;
		goto end;
	}
	
	if(stat(filename, &buf) < 0)
	{
		ret = -1;
		goto end;
	}	
	
	int filesize = buf.st_size;
	*content = (char *)malloc(filesize * sizeof(char));
	if(*content == NULL)
	{
		ret = -1;
		goto end;
	}
	
	if(fread(*content, filesize, 1, fp) != 1)
	{
		
		ret = -1;
		goto end;
	}
	ret = filesize;

end:
	return ret;
}