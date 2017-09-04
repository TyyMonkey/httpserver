#ifndef __PUB_H__
#define __PUB_H__

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

int socket_server_create(int port);
int socket_client_create();
int socket_accept(int fd);
int socket_connect(int fd, const char *ip, int port);
int setnonblock(int fd);
int readfile(const char *filename, char **content);

#endif