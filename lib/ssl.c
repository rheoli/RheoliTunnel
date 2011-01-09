#include "ssl.h"

int sslClientConnect(int _fd, SSL_CTX **_ctx, SSL **_ssl) {
  SSL_METHOD *meth;
  
  meth=SSLv23_client_method();
  *_ctx=SSL_CTX_new(meth);
  if (*_ctx == NULL) {
    printf("SSLClientSwitch: ctx error\n");
    return(0);
  }
  *_ssl=SSL_new(*_ctx);
  if (*_ssl == NULL) {
    printf("SSLClientSwitch: ssl_new error\n");
    return(0);
  }
  SSL_set_fd(*_ssl, _fd);
  if (SSL_connect(*_ssl) == -1) {
    printf("SSLClientSwitch: ssl_connect error\n");
    return(0);
  }
  return(1);
}

int sslServerConnect(int _fd, SSL_CTX **_ctx, SSL **_ssl) {
  SSL_METHOD *meth;
  SSL_CTX *test;
  meth=SSLv23_server_method();
  *_ctx=SSL_CTX_new(meth);
  if (*_ctx == NULL) {
    printf("SSLServerSwitch: ctx error\n");
    return(0);
  }
  if (SSL_CTX_use_certificate_file(*_ctx, "cert/cert.pem", SSL_FILETYPE_PEM) <= 0) {
    printf("SSL Certificate not found (cert.pem).\n");
    return(0);
  }
  if (SSL_CTX_use_PrivateKey_file(*_ctx, "cert/pkey.pem", SSL_FILETYPE_PEM) <= 0) {
    printf("SSL Private Key not found (pkey.pem).\n");
    return(0);
  }
  if (!SSL_CTX_check_private_key(*_ctx)) {
    printf("SSL Private Key wrong.\n");
    return(0);
  }
  *_ssl=SSL_new(*_ctx);
  if (*_ssl == NULL) {
    printf("SSLServerSwitch: ssl_new error\n");
    return(0);
  }
  SSL_set_fd(*_ssl, _fd);
  if (SSL_accept(*_ssl) == -1) {
    ERR_print_errors_fp(stderr);
    printf("SSLServerSwitch: ssl_accept error\n");
    return(0);
  }
  return(1);
}

int sslShutdown(SSL_CTX *_ctx, SSL *_ssl) {
  SSL_shutdown (_ssl);
  SSL_free (_ssl);
  SSL_CTX_free (_ctx);
}


int sslInit() {
  OpenSSL_add_ssl_algorithms();
  SSL_load_error_strings();
}

