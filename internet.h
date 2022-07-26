#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#ifdef __linux
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct linger      LINGER;
typedef struct hostent     HOSTENT;
typedef struct servent     SERVENT;
typedef struct protoent    PROTOENT;
typedef struct sockaddr    SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;

static int closesocket(int s_fd)
{
    close(s_fd);
    if (shutdown(s_fd, 2) == EOF) return EOF; 
    return 0;
}
#endif

#ifdef __WIN32
#include <winsock2.h>
typedef int socklen_t;
static int WSA_START() {WSADATA ws; WSAStartup(MAKEWORD(2, 2), &ws);}
static int WSA_STOP() {WSACleanup();}
#endif

static char *htoi(char *server_hostname)
{
    HOSTENT *h = gethostbyname(server_hostname);
    if (h == NULL) {return NULL;}
    char **p = h->h_addr_list;
    SOCKADDR_IN server_info;
    memcpy(&server_info.sin_addr, *p, sizeof(p));
    return inet_ntoa(server_info.sin_addr);
}

static SOCKADDR_IN sockaddr_info(char *host, int port)
{
    if (strcmp(host, "localhost") ||
        strcmp(host, "127.0.0.1") ||
        strcmp(host, "0.0.0.0") != 0) {
        host = htoi(host);
        if(host == NULL) {
            printf("DNS解析错误\n");
            exit(-1);
        }
    }

    SOCKADDR_IN server;
    #ifdef __linux
    server.sin_addr.s_addr = inet_addr(host);
    #endif
    #ifdef __WIN32
    server.sin_addr.S_un.S_addr = inet_addr(host);
    #endif
    server.sin_port        = htons(port);
    server.sin_family      = AF_INET;
    return server;
}

#ifndef __SOCKET_OBJECT_INIT__
#define __SOCKET_OBJECT_INIT__

static int TCP_Socket() {return socket(AF_INET, SOCK_STREAM, IPPROTO_IP);}
static int UDP_Socket() {return socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);}

static int CONNECT(int s_fd, SOCKADDR_IN info)
{
    if (connect(s_fd, (SOCKADDR *)&info, sizeof(info)) == EOF) return EOF;
    return 0;
}

static int BIND(int s_fd, int listen_n, SOCKADDR_IN info)
{
    int err[2] = {0, 0};
    err[0] = bind(s_fd, (SOCKADDR *)&info, sizeof(info));
    err[1] = listen(s_fd, listen_n);
    if(err[0] || err[1] == EOF) {return EOF;}
    return 0;
}

static int ACCEPT(int s_fd, SOCKADDR_IN info)
{
    socklen_t info_n = sizeof(info);
    int c = accept(s_fd, (SOCKADDR *)&info, &info_n);
    if (c == EOF) {return EOF;}
    return c;
}

static int SendData(int s_fd, void *data, u64 sbuf_n)
{
    if (send(s_fd, data, sbuf_n, 0) == EOF) {return EOF;}
    return 0;
}

static int RecvData(int s_fd, void *data, u64 rbuf_n)
{
    if (recv(s_fd, data, rbuf_n, 0) == EOF) {return EOF;}
    return 0;
}

static char *get_client_addr(SOCKADDR_IN info)
{
    return inet_ntoa(info.sin_addr);
}

static uint16_t get_client_port(SOCKADDR_IN info)
{
    return ntohs(info.sin_port);
}

#endif






