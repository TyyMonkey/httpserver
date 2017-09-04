#ifndef __WORK_H__
#define __WORK_H__

int make_http_resdponse(char *filename, char **pmsg, int *msglen);
void * thread_cb(void *arg);

#endif