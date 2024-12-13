#define MPL 256

struct client_req
{
    int number;
    char path1[MPL];
    char path2[MPL];
};

struct client_send
{
    int number;
    int  port;
    char ip[16];
};