#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "pub.h"
#include "work.h"

#define handle_error(msg) \
     do { perror(msg); exit(EXIT_FAILURE); } while (0)
     
//(ip.src == 192.168.1.104 && ip.dst == 192.168.1.143 && (tcp || http) && !ssh) || (ip.src == 192.168.1.143 && ip.dst == 192.168.1.104 && (tcp || http) && !ssh)

int main(int argc, char *args[])
{
	if(argc < 2)
	{
		printf("usage: port\n");
		return EXIT_SUCCESS;
	}
	
	pthread_t clientth;
	pthread_attr_t attr;
	int ret = pthread_attr_init(&attr);
	if(ret != 0)
		handle_error("pthread_attr_init");
		
	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if(ret != 0)
	{
		handle_error("pthread_attr_setdetachstate");
	}
	
	int listen_fd = socket_server_create(atoi(args[1]));
	if(listen_fd < 0)
	{
		handle_error("socket_server_create");
	}
	
	printf("http begin\n");

	int *ptmp = NULL;
	int client_fd = 0;
	while(1)
	{
		client_fd = socket_accept(listen_fd);
		
		if(client_fd < 0)
		{
			printf("socket_accept failed \n");
			return EXIT_FAILURE;
		}else
		{
			
			//创建连接线程
			ptmp = (int *)malloc(sizeof(int));
			*ptmp = client_fd;
			if(pthread_create(&clientth, &attr, &thread_cb, ptmp) < 0)
			{
				printf("pthread_create failed %s\n", strerror(errno));
			}
			
			/*
			char buf[1024] = {0};
			int rc = read(client_fd, buf, sizeof(buf));
			if(rc >= 0)
			{
				printf("buf = %s\n", buf);
			}
			close(client_fd);
			*/
		}	
	}
	pthread_attr_destroy(&attr);

	close(listen_fd);
	
	printf("http end\n");
	return EXIT_SUCCESS; 
}