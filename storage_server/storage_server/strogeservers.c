#include "basic_header.h"
#include "storageservers.h"
sem_t argsetter;
int sock;
entries entry;

void *client(void *arg)
{
    printf("hello\n");
    ssize_t s1, r1;
    struct com h = *(struct com *)arg;
    sem_post(&argsetter);
    char response[5];
    strcpy(response, "OK");
    printf("hiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii%d\n",h.h);
    if (h.h == READ)
    {
        printf("here wefrgfth\n");
        char *data = (char *)malloc(sizeof(char) * MAX_BUFFER_AT_ONE_TIME);
        int a = 11;
        int fd = open(h.arr, O_RDONLY);
        if (fd == -1)
        {
            perror("Error in opening file");
        }

        while (1)
        {
            printf("hi\n");
            a = readdata(data, fd);
            if (a != 0 && a != -1)
            {
                s1 = send(h.socket, data, MAX_BUFFER_AT_ONE_TIME, 0);
                if (s1 < 0)
                {
                    perror("Error in sending data");
                }

                r1 = recv(h.socket, response, 5, 0);
            }
            else
            {
                strcpy(data, "STOP");
                s1 = send(h.socket, data, MAX_BUFFER_AT_ONE_TIME, 0);
                r1 = recv(h.socket, response, 5, 0);
                break;
            }
        }

        close(fd);
    }
    else if (h.h == WRITE)
    {
        s1 = send(h.socket, response, 5, 0);

        char *data = (char *)malloc(sizeof(char) * MAX_BUFFER_AT_ONE_TIME);
        int fd = open(h.arr, O_WRONLY | O_TRUNC);
        if (fd == -1)
        {
            perror("Error in opening file");
        }

        while (1)
        {
            r1 = recv(h.socket, data, MAX_BUFFER_AT_ONE_TIME, 0);
            if (r1 < 0)
            {
                perror("Error in receiving data");
            }
            if (strcmp(data, "STOP") == 0)
            {   
                break;
            }
            // printf("Here-1");
            printf("%ld\n",strlen(data));
            writedata(data, fd);
            // printf("Here-2");
            int lol;

            s1 = send(h.socket, response, 5, 0);
            if (s1 < 0)
            {
                perror("Error in sending data");
            }
        }
        close(fd);
    }
    else if(h.h ==WRITE_ASYNC){
        printf("hereertydeswe 0\n");

        s1 = send(h.socket, response, 5, 0);

        int fd = open(h.arr, O_WRONLY | O_TRUNC);
        if (fd == -1)
        {
            perror("Error in opening file");
        }
        printf("hereertydeswe\n");
        
        char data[1000000];
        int a=0;
        data[a]='\0';
        while (1)
        {
            r1 = recv(h.socket, data+a, MAX_BUFFER_AT_ONE_TIME, 0);
            data[a+strlen(data+a)]='\0';
            if (r1 < 0)
            {
                perror("Error in receiving data");
            }
            if (strcmp(data+a, "STOP") == 0)
            {
                data[a]='\0';
                break;
            }

            s1 = send(h.socket, response, 5, 0);
            if (s1 < 0)
            {
                perror("Error in sending data");
            }
        
            a+=strlen(data+a);
        
        }
        int b=0;
        printf("hello\n");
        while(b<a){
            if(strlen(data+b)>MAX_BUFFER_AT_ONE_TIME){

            r1=write(fd,data+b,MAX_BUFFER_AT_ONE_TIME);
            }
            else{
                         r1=write(fd,data+b,strlen(data+b));
   
            }
            b+=r1;
        }
                    s1 = send(h.socket, response, 5, 0);
            if (s1 < 0)
            {
                perror("Error in sending data");
            }
        close(fd);
        }
    else if (h.h == ADD_INFO)
    {
        char *data = (char *)malloc(sizeof(char) * MAX_BUFFER_AT_ONE_TIME);
        additionalinfo(h.arr, data);
        s1 = send(h.socket, data, 1024, 0);
        if (s1 < 0)
        {
            perror("Error in sending data");
        }
        r1 = recv(h.socket, response, 5, 0);
    }
    else if (h.h== STREAM){
 printf("here wefrgfth eregtrhyew\n");
        char *data = (char *)malloc(sizeof(char) * MAX_BUFFER_AT_ONE_TIME);
        int a = 11;
        FILE* fd = fopen(h.arr, "rb");

        int d=0;
        while (1)
        {
            printf("hi1 %d\n",d);
            data[0]='\0';
            a = fread(data,1,MAX_BUFFER_AT_ONE_TIME-1,fd);
            data[a]='\0';

            printf("\n%d\n",a);

            if (a != 0 && a != -1)
            {
                printf("waiting to send\n");

                s1 = send(h.socket, data, a, 0);
                if (s1 < 0)
                {
                    perror("Error in sending data");
                }
                printf("waiting for response\n");
                r1 = recv(h.socket, response, 5, 0);
                printf("got response %s\n");
            }
            else
            {
                printf("hereee\n");
                strcpy(data, "STOP");
                s1 = send(h.socket, data, MAX_BUFFER_AT_ONE_TIME, 0);
                r1 = recv(h.socket, response, 5, 0);
                break;
            }
            d++;
        }
        fclose(fd);

    }
    strcpy(response, "OK");
    send(sock, response, 5, 0);
    return NULL;
}
int serverSocket11;
void doo(int port)
{
    int serverSocket11 = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket11 == -1)
    {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    printf("dfghjk\n");

    if (bind(serverSocket11, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    listen(serverSocket11, 3);

    return;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("REWRITE PORT\n");
        return 0;
    }
    int pp = atoi(argv[2]);
    char *ip = "10.2.139.65"; // Enter Name server IP here
    int port = 8080;
    int port1 = atoi(argv[1]);
    struct sockaddr_in addr;
    doo(pp);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    int a = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (a == -1)
    {
        perror("connection unsuccessful");
        return -1;
    }
    printf("Connected to the naming server.\n");
    int number = 8;
    send(sock, &number, 4, 0);
    recv(sock, &number, 4, 0);
    printf("%d\n", number);
    char cwd[1024];
    storageserverinitiailise(pp, ip, port1, &entry, getcwd(cwd, sizeof(cwd)));
    printf("%s\n",entry.pathsaccessible[0]);
    ssize_t s2 = send(sock, &entry, sizeof(entry), 0);
    if (s2 < 0)
    {
        perror("Error in sending data");
    }
    printf("%d %d\n", s2, sizeof(entry));
    // exit(0);
    // ptrtofirstdata datafirst = malloc(sizeof(struct firstdata));
    // recv(sock,datafirst,sizeof(firstdata),0);
    pthread_t fornamingserver;

    // printf("df");
    pthread_create(&fornamingserver, NULL, namingserver, &sock);
    // printf("FGhj\n");
    // recieve a struct of four integers two port numbers and one new or old
    // to naming server till here
    // pthread_join(fornamingserver,NULL);
    printf("hu\n");
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port1);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // printf("dfghjk\n");

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    sem_init(&argsetter, 0, 1);

    while (1)
    {
        printf("why seg-1\n");
        sem_wait(&argsetter);
        pthread_t threadid;

        printf("hi-1\n");
        listen(serverSocket, 5);
        printf("heid-1\n");
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        printf("hello-1\n");

        if (clientSocket == -1)
        {
            perror("accept");
            continue;
        }
        char dummy[10];
        recv(clientSocket, dummy, 10, 0);
        printf("%s\n", dummy);
        strcpy(dummy, "HI");
        send(clientSocket, dummy, 10, 0);
        struct com *h = malloc(sizeof(struct com));

        ssize_t r1 = recv(clientSocket, h, sizeof(struct com), 0);
        printf("%s %d\n", h->arr, h->h);
        h->socket = clientSocket;
        printf("hey\n");
        if (r1 < 0)
        {
            perror("Error in receiving data");
        }
        printf("heiii\n");
        pthread_create(&threadid, NULL, client, h);
        printf("I am resetting\n");
    }
}