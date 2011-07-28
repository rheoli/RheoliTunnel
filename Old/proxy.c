#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <sys/types.h>  // socket, bind
#include <sys/socket.h> // socket, bind
#include <netdb.h>
#include <unistd.h>
#include "lib/ssl.h"
#include "lib/net.h"

#define MAX_LINE 16

int main(int argc, char *argv[]) {
  int server_fd, rmt_fd, client_fd;
  struct sockaddr_storage rmt_addr;
  socklen_t rmt_addr_len;
  SSL_CTX *ctx;
  SSL     *ssl;
  int     need_ssl=0;
  SSL_CTX *client_ctx;
  SSL     *client_ssl;
  int     need_client_ssl=1;
  int     prozess_id=0;
  int     debug=1;

  struct pollfd pfd[2];
  char buf[BUFSIZ];
  int nfds;

  if (debug>0)
    printf("argc=%d\n", argc);
  if(argc>1) {
    if (debug>0)
      printf("argv[1]=%s\n", argv[1]);
    if(!strcmp(argv[1], "-ssl")) {
      need_ssl=1;
    }
  }

  if(need_ssl || need_client_ssl) {
    sslInit();
  }

  server_fd=netServer("443");

  while(1) {
    pid_t pid;

    rmt_addr_len=sizeof(rmt_addr);
    rmt_fd=accept(server_fd, (struct sockaddr *)&rmt_addr, &rmt_addr_len);

    prozess_id++;

    pid=fork();
    if(pid>0) {
      close(rmt_fd);
      continue;
    }
    close(server_fd);

    if (debug>0)
      printf("in child\n");

    client_fd=netClient("entry-vpn.swisslife.ch", "443");
    if(need_client_ssl) {
      if (sslClientConnect(client_fd, &client_ctx, &client_ssl)==0) {
        return(1);
      }
    }

    if(need_ssl) {
      if (sslServerConnect(rmt_fd, &ctx, &ssl)==0) {
        return(1);
      }
    }
    while(1) {
      int len;
      pfd[0].fd    =rmt_fd;
      pfd[0].events=POLLIN;
      pfd[1].fd    =client_fd;
      pfd[1].events=POLLIN;
      nfds = poll(pfd, 2, 2000);
      if (debug>0) {
        printf("EndOfWait (%d)...\n", nfds);
        printf("p0=%d, p1=%d\n", pfd[0].revents, pfd[1].revents);
      }
      if (nfds==-1) {
        printf(" -> nfds==-1\n");
        if (pfd[0].revents & (POLLERR|POLLHUP|POLLNVAL))
          break;
        if (pfd[1].revents & (POLLERR|POLLHUP|POLLNVAL))
          break;
      }
      if (nfds>0) {
        if (pfd[0].revents!=0) {
        int  i;
        char hex_string[MAX_LINE+1];
          if (debug>0)
            printf("Read from server (%d)\n", pfd[0].revents);
          if(need_ssl)
            len=SSL_read(ssl, &buf, sizeof(buf)-1);
          else
            len=read(rmt_fd, &buf, sizeof(buf)-1);
          if(len<0) {
            printf(" -> server error?\n");
            break;
          }
          if(len==0) {
            printf(" -> server hung up\n");
            break;
          }
        for(i=0; i<=len; i++) {
          if(i%MAX_LINE==0 || i==len) {
            if(i>0) {
              hex_string[MAX_LINE]='\0';
              printf("|%s|\n", hex_string);
            }
            if(i==len)
              break;
            printf("%02d|>C|", prozess_id);
          }
          printf(" %02x", buf[i]);
          hex_string[i%MAX_LINE]=buf[i];
          if(buf[i]<32)
            hex_string[i%MAX_LINE]='.';
        }
          if(len>0) {
            if(need_client_ssl) {
              int wrote;
              wrote=SSL_write(client_ssl, buf, len); 
              printf("Wrote %d chars to client\n", wrote);
            }
            else {
              write(client_fd, buf, len); 
            }
          }
        }
      if (pfd[1].revents!=0) {
        int  i;
        char hex_string[MAX_LINE+1];
        if (debug>0)
          printf("Read from client (%d)\n", pfd[1].revents);
        if(need_client_ssl)
          len=SSL_read(client_ssl, &buf, sizeof(buf)-1);
        else
          len=read(client_fd, &buf, sizeof(buf)-1);
        if (len<0) {
          printf(" -> client error?\n");
          break;
        }
        if(len==0) {
          printf(" -> client hung up\n");
          break;
        }
        if (debug>0)
          printf(" - Read lines (%d) (%s)\n", len, buf); 
        for(i=0; i<len; i++) {
          if(i%MAX_LINE==0 || i==len) {
            if(i>0) {
              hex_string[MAX_LINE]='\0';
              printf("|%s|\n", hex_string);
            }
            if(i==len)
              break;
            printf("%02d|C>|", prozess_id);
          }
          printf(" %02x", buf[i]);
          hex_string[i%MAX_LINE]=buf[i];
          if(buf[i]<32)
            hex_string[i%MAX_LINE]='.';
        }
        if (len>0) {
          if(need_ssl)
            SSL_write(ssl, buf, len); 
          else
            write(rmt_fd, buf, len); 
        }
      }
    }
  }

  if(need_ssl)
    sslShutdown(ctx, ssl);
  if(need_client_ssl)
    sslShutdown(client_ctx, client_ssl);
  close(rmt_fd);
  close(client_fd);
  exit(0);

  }

  close(server_fd);

  return(0);
}


