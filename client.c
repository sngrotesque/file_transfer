#include "file_transfer.h"

// * da31379e0a7f74f8e2e9cf0bd9f76d7fd1923536a7283feb8385d9dd1a8d88f1 * //

int main(int argc, char **argv)
{
    char *host = "192.168.31.111";
    uint16_t port = 8080;
    FILE *fp = fopen("./out/bilibili_2233.jpeg", "rb");

    int err = sender_handler(host, port, fp);
    printf("错误代码: %d\n", err);

    fclose(fp);
    return 0;
}

