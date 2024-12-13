#include "basic_header.h"
#include "macros.h"
#include "header.h"
#include "Main_semaphore.h"
#include "function_def.h"

void* set_up_tcp_user(void *)
{
    sem_wait(&main_s);
    int server_fd, new_socket;
    int reuse_opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t request_thread[MAX_USER_REQ]; // Arr of threads

    // Socket formation
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Enabe Socket Reuse
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &reuse_opt, sizeof(reuse_opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(User_Port);

    // Binding Port to socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, Max_Req_Queue) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    sem_init(&user_s, 0, 1);
    sem_post(&main_s);
    while (1)
    {
        printf("herefhgjfkls\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("I am here\n");
        if (pthread_create(&request_thread[i], NULL, handle_user, (void *)&new_socket) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
        // pthread_join(request_thread[i],NULL);

        i++;
    }
}