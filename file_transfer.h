#include "internet.h"

#define BlockSize 4096
#define SOCK_SIGNAL_BEGIN (uint8_t *)"233333"
#define SOCK_SIGNAL_END   (uint8_t *)"233"

#ifdef __linux
typedef int SOCKET;
#endif

typedef struct {
    size_t total;
    size_t residual;
} group_size;

static int64_t get_file_size(FILE *stream)
{
    int64_t file_size = -1;
    int64_t cur_offset = ftell(stream);
    fseek(stream, 0, SEEK_END);
    file_size = ftell(stream);
    fseek(stream, cur_offset, SEEK_SET);
    return file_size;
}

static int receiver_handler(char *host, uint16_t port, FILE *fp)
{
    SOCKET server_socket, client_socket;
    SOCKADDR_IN server_info, client_info;
    uint8_t data[4097] = {0};
    group_size num = {0, 0};
    uint32_t TIMEOUT = 1;
    size_t file_size = 0;
    char *ptr = NULL;

    server_socket = TCP_Socket();
    server_info = sockaddr_info(host, port);
    if(server_socket == EOF) {return EOF;}

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&TIMEOUT, sizeof(TIMEOUT));

    BIND(server_socket, 5, server_info);
    client_socket = ACCEPT(server_socket, client_info);
    if(client_socket == EOF) return EOF;

    RecvData(client_socket, data, 20);
    file_size = strtoll((char *)data, &ptr, 16);
    group_size size_num = {
        file_size / BlockSize,
        file_size % BlockSize
    };

    for (size_t x = 0; x < size_num.total; ++x) {
        RecvData(client_socket, data, BlockSize);
        fwrite(data, 1, BlockSize, fp);
    }
    memset(data, 0, BlockSize);
    RecvData(client_socket, data, size_num.residual);
    fwrite(data, 1, size_num.residual, fp);

    closesocket(client_socket);
    closesocket(server_socket);
    return 0;
}

static int sender_handler(char *host, uint16_t port, FILE *fp)
{
    SOCKET client_socket = TCP_Socket();
    SOCKADDR_IN server_info = sockaddr_info(host, port);
    size_t file_size = get_file_size(fp);
    char send_file_size_buf[21];
    uint8_t data[4097] = {0};
    group_size size_num = {
        file_size / BlockSize,
        file_size % BlockSize
    };

    if (client_socket == EOF) return EOF;
    if (CONNECT(client_socket, server_info) == EOF) return EOF;

    sprintf(send_file_size_buf, "%020lx", file_size);
    SendData(client_socket, send_file_size_buf, 20);

    for (size_t x = 0; x < size_num.total; ++x) {
        fread(data, 1, BlockSize, fp);
        SendData(client_socket, data, BlockSize);
    }
    memset(data, 0, BlockSize);
    fread(data, 1, size_num.residual, fp);
    SendData(client_socket, data, size_num.residual);

    closesocket(client_socket);
    return 0;
}


