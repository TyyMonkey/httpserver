#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "work.h"
#include "pub.h"
#include "myhttp.h"

#define EN_PRINT_RECV_ROW_DATA 1U
#define EN_PRINT_SEND_ROW_DATA 1U

/*
a.消息头内容格式：（字符串）
HTTP/1.0 200 OK\n
Content-Type: image/x-icon\n（标识给客户端回复的消息体是什么类型的文件，消息体是图像文件，图像是icon格式的）
Transfer-Encoding: chunked\n
Connection: Keep-Alive\n
Accept-Ranges:bytes\n
Content-Length:2550\n \n（消息体的长度，单位：字节）

b.消息体：（二进制内容）具体要给客户端发送的文件二进制流

c.消息尾（字符串）：\n\n(两个换行符)
*/
/*
#define HEAD "HTTP/1.0 200 OK\n\
Content-Type: %s\n\
Transfer-Encoding: chunked\n\
Connection: Keep-Alive\n\
Accept-Ranges:bytes\n\
Content-Length:%d\n\n"
*/

#define HEAD "HTTP/1.0 200 OK\n\
Content-Type: %s\n\
Connection: Keep-Alive\n\
Accept-Ranges:bytes\n\
Content-Length:%d\n\n"

#define TAIL "\n\n"

#define EXEC "s?wd="

void * thread_cb(void *arg)
{
	
	int fd = *(int *)arg;
	free((int *)arg);
	
	char buf[2014] = {0};
	int rc = recv(fd, buf, sizeof(buf), 0);
	
	if(rc > 0)
	{
#if EN_PRINT_RECV_ROW_DATA > 0U
		printf("\n****************************************\n");
		printf("%s\n", buf);
		printf("\n****************************************\n");
#endif
		char *filename = NULL;
	  if(myhttp_get_filename(buf, &filename)  < 0)
	  {
	  	printf("analyse_request failed\n");
	  	goto end;
	  }

  	//repsnse
  	char *msg = NULL;
  	int msglen = 0;
  	
  	if(make_http_resdponse(filename,  &msg, &msglen) < 0)
  	{
  		printf("make_http_resdponse failed\n");
  		goto end;
  	}
  	free(filename);
#if EN_PRINT_SEND_ROW_DATA > 0U
		printf("\n****************************************\n");
		printf("%s\n", msg);
		printf("\n****************************************\n");
#endif
  	if(send(fd, msg, msglen, 0) < 0)
  	{
  		printf("send failed %s\n", strerror(errno));
  		goto end;
  	}
  	free(msg);
	}
	
end:	
	close(fd);
	
	return NULL;
}

int make_http_resdponse(char *filename, char **pmsg, int *msglen)
{
	int ret = 0;
	char *p = NULL;
	char *content = NULL;
	int headlen = 0;//消息头长度
	int taillen = strlen(TAIL);//消息尾长度
	int contentlen = 0;//消息体长度
	if( NULL == filename || NULL == msglen )
	{
		ret = -1;
		goto end;
	}
	
	if(strcmp(filename, "") == 0)
	{
		printf("transmit index.html\n");
		//传输默认网页
		if( (contentlen = readfile("index.html", &content)) < 0)
		{
			ret = -1;
			goto end;
		}
	}else if(strcmp(filename, EXEC) == 0)	
	{
		//执行指令指定文件
	}else
	{
		printf("transmit %s\n", filename);
		//传输指定文件
		if( (contentlen = readfile(filename, &content)) < 0)
		{
			printf("readfile failed!\n");
			ret = -1;
			goto end;
		}
	}
	//拼接消息
	if(content != NULL)
	{
		char buf[1024] = {0};
		//获取消息头
		sprintf(buf, HEAD, myhttp_gettype(filename), contentlen);
    headlen = strlen(buf);
    
    //创建缓存
    p = (char *)malloc((headlen + contentlen + taillen) * sizeof(char));
    if(NULL == p)
		{
			ret = -1;
			goto end;
		}
		
		//填充消息头
		memcpy(p, buf, headlen);
		//填充消息体
		memcpy(p + headlen, content, contentlen);
		//填充消息尾
		memcpy(p + headlen + contentlen, TAIL, taillen);
		printf("1\n");
		
		*pmsg = p;
		*msglen = headlen + contentlen + taillen;
		
		//释放资源
		free(content);
	}else{
		ret = -1;
		goto end;
	}
	
end:
		return ret;
}