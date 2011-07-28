#include "net.h"

int netServer(const char *_port) {
  int sock_fd, rmt_fd;
  struct addrinfo hints, *res;
  int yes='1';

  memset(&hints, 0, sizeof(hints));
  hints.ai_family  =AF_UNSPEC;
  hints.ai_socktype=SOCK_STREAM;
  hints.ai_flags   =AI_PASSIVE;  
  getaddrinfo(NULL, _port, &hints, &res);

  sock_fd=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sock_fd == -1) {
    fprintf(stderr, "Error in socket()\n");
    return(-1);
  }
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    fprintf(stderr, "Error in setsockopt()\n");
    return(-1);
  }
  if (bind(sock_fd, res->ai_addr, res->ai_addrlen) != 0) {
    fprintf(stderr, "Error in bind()\n");
    return(-1);
  }
  listen(sock_fd, 5);

  return(sock_fd);
}

int netClient(const char *_host, const char *_port) {
  int sock_fd;
  struct addrinfo hints, *res;

  memset(&hints, 0, sizeof(hints));
  //hints.ai_family  =AF_UNSPEC;
  hints.ai_family  =AF_INET;
  hints.ai_socktype=SOCK_STREAM;
  getaddrinfo(_host, _port, &hints, &res);

  sock_fd=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sock_fd == -1) {
    fprintf(stderr, "Error in client socket()\n");
    return(-1);
  }
  printf("Connect to %s\n", res->ai_addr);
  if (connect(sock_fd, res->ai_addr, res->ai_addrlen)==-1) {
    fprintf(stderr, "Error in client connect()\n");
    return(1);
  }
  return(sock_fd);
}

