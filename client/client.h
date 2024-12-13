#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include<sys/wait.h>



#define MAX 1024
#define MPL 512
#define or ||


// STRUCTS

typedef struct forread
{
    char *data;
    int len;
} forread;

typedef struct storageserver
{
    int flag;
    int port;
    char ip[16];
}storageserver;

struct namingserver
{
    int number;
    char path1[MPL];
    char path2[MPL];
};


// FUNCTIONS

int get_sockfd(struct sockaddr_in server_addr, char *ip, int port_no);
size_t send_sock(char *host_ip, int port_no, struct namingserver *NS);
storageserver recieve_data(char *host_ip, int port);
void handle_flags(char* host_ip,int port1, struct storageserver *SS,struct namingserver *NS,int nssock);
void book(char *str);



// OPERATIONS


#define CREATE_FILE 1
#define CREATE_DIR 2
#define DELETE_FILE 3
#define DELETE_DIR 4
#define COPY_FILE 5
#define COPY_DIR 6
#define READ 7
#define WRITE 8
#define ADD_INFO 9
#define STREAM 10
#define WRITE_ASYNC 11
#define LIST_PATHS 15
