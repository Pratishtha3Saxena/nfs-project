#include "client.h"

int get_sockfd(struct sockaddr_in server_addr, char *ip, int port_no)
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("Error in socket creation for CLIENT");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_no);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Problem in connecting to the naming server");
        exit(EXIT_FAILURE);
    }

    return sock_fd;
}

size_t send_sock(char *host_ip, int port_no, struct namingserver *NS)
{
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);
    struct storageserver ss;
    int fd1;
    fd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (fd1 < 0)
    {
        perror("Error in socket creation for CLIENT");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_no);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int b = connect(fd1, (struct sockaddr *)&server_addr, sizeof(server_addr));
    

}

storageserver recieve_data(char *host_ip, int port)
{
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

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(host_ip);
    int b = bind(fd1, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (b < 0)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    listen(fd1, 5);

    fd2 = accept(fd1, (struct sockaddr *)&client_addr, &len);
    if (fd2 < 0)
    {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_received = recv(fd2, &ss, sizeof(struct storageserver), 0);
    if (bytes_received < 0)
    {
        perror("Error receiving data from server");
        exit(EXIT_FAILURE);
    }

    
    close(fd1);
    close(fd2);

    return ss;
}