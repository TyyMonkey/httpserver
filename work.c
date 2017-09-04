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
a.��Ϣͷ���ݸ�ʽ�����ַ�����
HTTP/1.0 200 OK\n
Content-Type: image/x-icon\n����ʶ���ͻ��˻ظ�����Ϣ����ʲô���͵��ļ�����Ϣ����ͼ���ļ���ͼ����icon��ʽ�ģ�
Transfer-Encoding: chunked\n
Connection: Keep-Alive\n
Accept-Ranges:bytes\n
Content-Length:2550\n \n����Ϣ��ĳ��ȣ���λ���ֽڣ�

b.��Ϣ�壺�����������ݣ�����Ҫ���ͻ��˷��͵��ļ���������

c.��Ϣβ���ַ�������\n\n(�������з�)
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
	int headlen = 0;//��Ϣͷ����
	int taillen = strlen(TAIL);//��Ϣβ����
	int contentlen = 0;//��Ϣ�峤��
	if( NULL == filename || NULL == msglen )
	{
		ret = -1;
		goto end;
	}
	
	if(strcmp(filename, "") == 0)
	{
		printf("transmit index.html\n");
		//����Ĭ����ҳ
		if( (contentlen = readfile("index.html", &content)) < 0)
		{
			ret = -1;
			goto end;
		}
	}else if(strcmp(filename, EXEC) == 0)	
	{
		//ִ��ָ��ָ���ļ�
	}else
	{
		printf("transmit %s\n", filename);
		//����ָ���ļ�
		if( (contentlen = readfile(filename, &content)) < 0)
		{
			printf("readfile failed!\n");
			ret = -1;
			goto end;
		}
	}
	//ƴ����Ϣ
	if(content != NULL)
	{
		char buf[1024] = {0};
		//��ȡ��Ϣͷ
		sprintf(buf, HEAD, myhttp_gettype(filename), contentlen);
    headlen = strlen(buf);
    
    //��������
    p = (char *)malloc((headlen + contentlen + taillen) * sizeof(char));
    if(NULL == p)
		{
			ret = -1;
			goto end;
		}
		
		//�����Ϣͷ
		memcpy(p, buf, headlen);
		//�����Ϣ��
		memcpy(p + headlen, content, contentlen);
		//�����Ϣβ
		memcpy(p + headlen + contentlen, TAIL, taillen);
		printf("1\n");
		
		*pmsg = p;
		*msglen = headlen + contentlen + taillen;
		
		//�ͷ���Դ
		free(content);
	}else{
		ret = -1;
		goto end;
	}
	
end:
		return ret;
}