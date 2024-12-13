#include "client.h"
#define Max_SS_no 1024
#define MAX_PATHS 256
#define MAX_PATH_LENGTH 256
#define MAX_BUFFER_AT_ONE_TIME 1024
struct com
{
    int h;
    char arr[1024];
    int socket;
    char arr1[1024];
};
int dff=0;
void* handle_write_async(void *arg)
{
        printf("dvxz");

    int client_socket = *((int *)arg);
    char data[MAX_BUFFER_AT_ONE_TIME];
    int a=-1;
    printf("dvxz");
    while(a==-1 || a==0){
        a=recv(client_socket,data,MAX_BUFFER_AT_ONE_TIME,MSG_DONTWAIT);
    }
    dff=1;
}
void handle_flags(char *host_ip, int port, struct storageserver *SS, struct namingserver *NS, int nssock)
{
    if (SS->flag == 0)
    {
        char *ip = SS->ip;
        printf("SS IP: %s\n", SS->ip);
        printf("hello\n");
        printf("Connecting to another server on port %d...\n", SS->port);
        book("Connecting to storage server");
        // send_sock(host_ip, SS->port, NS);
        int sock;
        struct sockaddr_in addr;

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            perror("[-]Socket error");
            exit(1);
        }
        printf("[+]TCP server socket created.\n");

        memset(&addr, '\0', sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(SS->port);
        addr.sin_addr.s_addr = inet_addr(ip);

        int a = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
        if (a == -1)
        {
            perror("connection unsuccessful");
            return;
        }
        printf("Connected to the storage server.\n");
        book("Connected to the storage server");

        char dummy[10];
        strcpy(dummy, "HELLO");
        int s1 = send(sock, dummy, 10, 0);
        if (s1 < 0)
        {
            perror("Error in sending data");
        }
        int r1 = recv(sock, dummy, 10, 0);
        if (r1 < 0)
        {
            perror("Error in receiving data");
        }
        // printf("%s\n", dummy);
        struct com *h = (struct com *)malloc(sizeof(struct com));
        strcpy(h->arr, NS->path1);
        h->h = NS->number;
        // printf("%s %d\n", h->arr, h->h);
        int s2 = send(sock, h, sizeof(struct com), 0);
        if (s2 < 0)
        {
            perror("Error in sending data");
        }
        char data[MAX_BUFFER_AT_ONE_TIME];
        char response[5];
        strcpy(response, "OK");

        if (NS->number == READ || NS->number == STREAM)
        {
            int a = 0;
            pid_t pid = 1;
            int pipe_fd[2];

            if (NS->number == STREAM)
            {

                if (pipe(pipe_fd) == -1)
                {
                    perror("Pipe creation failed");
                    exit(EXIT_FAILURE);
                }
                pid = fork();
                if (pid == -1)
                {
                    exit(EXIT_FAILURE);
                }
            }
            if (pid == 0)
            {
                
                close(pipe_fd[1]);              // Close write end of the pipe
                dup2(pipe_fd[0], STDIN_FILENO); // Redirect pipe's read end to stdin
                close(pipe_fd[0]);
                // printf("OOSJO\n");
                char *argument[] = {"/usr/bin/mpv", "--no-video","--no-terminal" ,"-", NULL};

                execvp("mpv", argument);
                // printf("YES\n");
                perror("mpv execution failed");
                exit(EXIT_FAILURE);
            }
            else
            {
                if (NS->number == STREAM)
                {
                    
                    close(pipe_fd[0]); // Close write end of the pipe
                }

                while (1)
                {
                    int r2 = recv(sock, data, MAX_BUFFER_AT_ONE_TIME, 0);
                    // printf("hereee i received something\n");
                    if (r2 < 0)
                    {
                        perror("Error in receiving data");
                    }

                    if (strcmp(data, "STOP") == 0)
                    {
                        book("Data given\n");
                        if (NS->number == READ)
                        {

                            printf("\n");
                        }
                        int s3 = send(sock, response, 5, 0);
                        if (s3 < 0)
                        {
                            perror("Error in sending data data");
                        }
                        break;
                    }
                    else if (strcmp(data, "STOP1") == 0)
                    {
                        if (NS->number == READ)
                        {

                            printf("\n");
                        }
                        int s3 = send(sock, response, 5, 0);
                        if (s3 < 0)
                        {
                            perror("Error in sending data data");
                        }
                        printf("Error in reading\n");
                        book("Error in reading\n");
                        break;
                    }
                    if (NS->number == READ)
                    {

                        printf("%s", data);
                    }
                    else
                    {
                        // printf("%d %s\n", r2, data);
                        // sleep(1);
                        write(pipe_fd[1], data, r2);
                        // printf("write hogya \n");
                    }
                    int s4 = send(sock, response, 5, 0);
                    if (s4 < 0)
                    {
                        perror("Error in sending data");
                    }
                    data[0] = '\0';
                    a++;
                }
                if (NS->number == STREAM)
                {
                    close(pipe_fd[1]);
                    waitpid(pid, NULL, 0);
                }
                // printf("\n");                // printf("\n");

            } // printf("%d\n", a);
        }
        else if (NS->number==WRITE_ASYNC){
            char data[MAX_BUFFER_AT_ONE_TIME*2];
            char response[5];

            int r3 = recv(sock, response, 5, 0);
            if (r3 < 0)
            {
                perror("Error in receiving data");
            }
            int packetNumber = 1;
            size_t bytesRead;
            int fl = 0;
            printf("hi how are you\n");
            while (1)
            {
                printf("hiiiiiii\n");
                if (fgets(data, MAX_BUFFER_AT_ONE_TIME, stdin) != NULL)
                {
                    // printf("hereee\n");
                    // char *newline_pos = strchr(data, '\n');
                    // if (newline_pos != NULL)
                    // {
                    //     *newline_pos = '\0';
                    //     bytesRead = newline_pos - data + 1;
                    //     printf("%d\n",bytesRead);
                    //     printf("hello\n");
                    //     int s5 = send(sock, data, MAX_BUFFER_AT_ONE_TIME, 0);
                    //     if (s5 < 0)
                    //     {
                    //         perror("Error in sending data");
                    //     }
                    //                             printf("heloooooooo\n");

                    //     int r4 = recv(sock, response, 5, 0);
                    //     if (r4 < 0)
                    //     {
                    //         perror("Error in recieving data");
                    //     }
                    //                             printf("heloooooooo\n");


                    //     break;
                        
                    //     printf("heloooooooo\n");
                    // }
                    // else
                    // {
                        bytesRead = strlen(data);
                        data[bytesRead]='\0';
                        printf("%s %d\n",data,bytesRead);
                        if(strcmp(data,"STOP\n")==0){
                            break;
                        }
                        int s6 = send(sock, data, MAX_BUFFER_AT_ONE_TIME, 0);
                        if (s6 < 0)
                        {
                            perror("Error in sending data");
                        }
                        data[0]='\0';
                    // }
                }
                else
                {
                    fl = 1;
                    break;
                }
                int r5 = recv(sock, response, 5, 0);
                if (r5 < 0)
                {
                    perror("Error in recieving data");
                }
                if (strcmp(response, "ERROR") == 0)
                {
                    fl = 2;
                    break;
                }
            }

            if (fl != 2)
            {
                strcpy(data, "STOP");
                int s7 = send(sock, data, strlen("STOP") + 1, 0);
                if (s7 < 0)
                {
                    perror("Error in sending data");
                }
                printf("hryy2\n");

                pthread_t request;
                pthread_create(&request,NULL,handle_write_async,(void *)&sock);
                while (dff==0)
                {
                    printf("I define variables named mbappe messi ronaldo :),iykyk\n");
                }
                printf("\n\n\n\nI recieved an acknowledgement of my write completing\n");
                
                book("Write done successfully\n");
            }
            if (fl == 1)
            {
                printf("error in fgets\n");
                book("error in fgets\n");
            }
            if (fl == 2)
            {
                printf("Error in writing\n");
                book("Error in writing\n");
            }

        }
        else if (NS->number == WRITE)
        {

            char data[MAX_BUFFER_AT_ONE_TIME];
            char response[5];

            int r3 = recv(sock, response, 5, 0);
            if (r3 < 0)
            {
                perror("Error in receiving data");
            }
            int packetNumber = 1;
            size_t bytesRead;
            int fl = 0;

            while (1)
            {
                if (fgets(data, MAX_BUFFER_AT_ONE_TIME, stdin) != NULL)
                {
                    char *newline_pos = strchr(data, '\n');
                    if (newline_pos != NULL)
                    {
                        *newline_pos = '\0';
                    printf("Remaining data:%s\n",data);
                        printf("I am here\n");
                        bytesRead = newline_pos - data + 1;
                        int s5 = send(sock, data, MAX_BUFFER_AT_ONE_TIME, 0);

                        if (s5 < 0)
                        {
                            perror("Error in sending data");
                        }
                        int r4 = recv(sock, response, 5, 0);
                        if (r4 < 0)
                        {
                            perror("Error in recieving data");
                        }
                        printf("Response: %s\n", response);
                        // memset(data, 0, sizeof(data));
                        break;
                    }
                    else
                    {
                        printf("Useless:%s",data);
                        data[1023]='\0';
                        bytesRead = strlen(data) + 1;
                        printf("Useless bytes:%d\n",bytesRead);
                        int s6 = send(sock, data, MAX_BUFFER_AT_ONE_TIME, 0);
                        if (s6 < 0)
                        {
                            perror("Error in sending data");
                        }
                        

                    }
                }
                else
                {
                    fl = 1;
                    break;
                }
                int r5 = recv(sock, response, 5, 0);
                printf("Response %s\n",response);
                if (r5 < 0)
                {
                    perror("Error in recieving data");
                }
                if (strcmp(response, "ERROR") == 0)
                {
                    fl = 2;
                    break;
                }
            }
            if (fl != 2)
            {
                printf("hereee\n");
                strcpy(data, "STOP");
                int s7 = send(sock, data, MAX_BUFFER_AT_ONE_TIME, 0);
                if (s7 < 0)
                {
                    perror("Error in sending data");
                }
                book("Write done successfully\n");
            }
            if (fl == 1)
            {
                printf("error in fgets\n");
                book("error in fgets\n");
            }
            if (fl == 2)
            {
                printf("Error in writing\n");
                book("Error in writing\n");
            }
        }
        else if (NS->number == ADD_INFO)
        {
            int r6 = recv(sock, data, MAX_BUFFER_AT_ONE_TIME, 0);
            if (r6 < 0)
            {
                perror("Error in recieving data");
            }
            if (strcmp(data, "INVALID") == 0)
            {
                printf("Invalid path or stat doesn't exist\n");
                book("Invalid path or stat doesn't exist\n");
            }
            else
                printf("%s\n", data);
            int s8 = send(sock, response, 5, 0);
            if (s8 < 0)
            {
                perror("Error in sending data");
            }
        }
    }
    else if (SS->flag == 2)
    {
        char buffer[8192];
        int r7 = recv(nssock, buffer, 8192, 0);
        if (r7 < 0)
        {
            perror("Error in recieving data");
        }
        printf("%s\n", buffer);
        book(buffer);
    }
    else
    {
        printf("%d\n", SS->flag);
        printf("Waiting for more data from the initial server...\n");
        {
            char buffer[8192];
            int r8 = recv(nssock, buffer, 8192, 0);
            if (r8 < 0)
            {
                perror("Error in recieving data");
            }

            printf("%s\n", buffer);
            book(buffer);
        }
    }
}