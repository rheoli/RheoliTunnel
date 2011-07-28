#ifndef __H_SSL
#define __H_SSL

#include <stdio.h>
#include <string.h>
#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rand.h>

int sslClientConnect(int _fd, SSL_CTX **_ctx, SSL **_ssl);
int sslServerConnect(int _fd, SSL_CTX **_ctx, SSL **_ssl);
int sslShutdown(SSL_CTX *_ctx, SSL *_ssl);
int sslInit();

#endif
