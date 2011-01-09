#ifndef __H_NET
#define __H_NET

#include <stdio.h>
#include <string.h>
#include <sys/types.h>  // socket, bind
#include <sys/socket.h> // socket, bind
#include <netdb.h>
#include <unistd.h>

int netServer(const char *_port);
int netClient(const char *_host, const char *_port);

#endif
