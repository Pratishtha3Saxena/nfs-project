#include "client.h"

int main()
{
    char input[MAX];
    fgets(input, sizeof(input), stdin);
    // printf("%s\n",input);
    struct namingserver NS;
    storageserver SS;
    char *x;
    book(input);
    char *oper = strtok_r(input, " ", &x);
    if (strcmp("LIST\n", oper) == 0) {
        NS.number = LIST_PATHS;
    }
    else if (strcmp("READ", oper) == 0)
    {
        NS.number = READ;
        char *h1 = strtok_r(NULL, "\n", &x);
        strcpy(NS.path1, h1);
    }
    else if (strcmp("WRITE", oper) == 0)
    {
        NS.number = WRITE;
        char *h1 = strtok_r(NULL, "\n", &x);
        strcpy(NS.path1, h1);
    }
    else if (strcmp("CREATE_FILE", oper) == 0)
    {
        NS.number = CREATE_FILE;
        char *h1 = strtok_r(NULL, "\n", &x);
        printf("H1: %s\n", h1);
        strcpy(NS.path1, h1);
    }
    else if (strcmp("CREATE_DIR", oper) == 0)
    {
        NS.number = CREATE_DIR;
        char *h1 = strtok_r(NULL, "\n", &x);
        strcpy(NS.path1, h1);
    }
    else if (strcmp("COPY_FILE", oper) == 0)
    {
        NS.number = COPY_FILE;
        char *h1 = strtok_r(NULL, " ", &x);
        char *h2 = strtok_r(NULL, "\n", &x);
        strcpy(NS.path1, h1);
        strcpy(NS.path2, h2);
    }
    else if (strcmp("COPY_DIR", oper) == 0)
    {
        NS.number = COPY_DIR;
        char *h1 = strtok_r(NULL, " ", &x);
        char *h2 = strtok_r(NULL, "\n", &x);
        strcpy(NS.path1, h1);
        strcpy(NS.path2, h2);
    }
    else if (strcmp("DELETE_DIR", oper) == 0)
    {
        NS.number = DELETE_DIR;
        char *h1 = strtok_r(NULL, "\n", &x);
        strcpy(NS.path1, h1);
    }
    else if (strcmp("DELETE_FILE", oper) == 0)
    {
        NS.number = DELETE_FILE;
        char *h1 = strtok_r(NULL, "\n", &x);
        strcpy(NS.path1, h1);
    }
    else if (strcmp("ADDITIONAL_INFO", oper) == 0)
    {
        NS.number = ADD_INFO;
        char *h1 = strtok_r(NULL, "\n", &x);
        strcpy(NS.path1, h1);
    }
    else if (strcmp("STREAM", oper) == 0)
    {
        NS.number = STREAM;
        char *h1 = strtok_r(NULL, "\n", &x);
        strcpy(NS.path1, h1);
        if (NS.path1[strlen(NS.path1) - 1] == 't')
        {
            printf("INVALID FILE FORMAT\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp("WRITE_ASYNC", oper) == 0)
    {
        NS.number = WRITE_ASYNC;
        char *h1 = strtok_r(NULL, "\n", &x);
        strcpy(NS.path1, h1);
    }

    int fd1, fd2;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);
    struct storageserver ss;

    fd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (fd1 < 0)
    {
        perror("Error in socket creation for CLIENT");
        exit(EXIT_FAILURE);
    }
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd1, &readfds);

    int ready = select(fd1 + 1, &readfds, NULL, NULL, &timeout);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5555);
    server_addr.sin_addr.s_addr = inet_addr("10.2.139.65"); // Enter Name server IP
    printf("I am here\n");
    int b = connect(fd1, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (b < 0)
    {
        perror("Connecting Unsuccessfully");
        exit(EXIT_FAILURE);
    }

    printf("CONNECTED TO NAMING SERVER\n");
    int flag = 0;
    if (NS.number == STREAM)
    {
        NS.number = READ;
        flag = 1;
    }
    else if (NS.number == WRITE_ASYNC)
    {
        NS.number = WRITE;
        flag = 1;
    }
    ssize_t bytes_sent = write(fd1, &NS, sizeof(struct namingserver));
    ssize_t bytes_received = recv(fd1, &ss, sizeof(struct storageserver), 0);
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    if (bytes_received != sizeof(struct storageserver))
    {
        printf("timeout!!\n");
        book("timeout\n");
        perror("Error receiving data from server");
        exit(EXIT_FAILURE);
    }
    book("Connection done\n");
    printf("HERE\n");
    if (ss.flag == 1)
    {
        printf("okokoo\n");
        int u = 1;
        bytes_sent = send(fd1, &u, 4, 0);
    }
    if (flag == 1)
    {
        if (NS.number == READ)
        {

            NS.number = STREAM;
        }
        else
        {
            NS.number = WRITE_ASYNC;
        }
    }

    handle_flags("127.0.0.1", 5555, &ss, &NS, fd1);
}