#include "file_transfer.h"

int main(int argc, char **argv)
{
    char *host = "0.0.0.0";
    uint16_t port = 8890;
    FILE *fp = fopen("./TESSSSSSSSSSSSSSSSSSSST", "wb");

    int err = receiver_handler(host, port, fp);
    printf("错误代码: %d\n", err);

    fclose(fp);
    return 0;
}

