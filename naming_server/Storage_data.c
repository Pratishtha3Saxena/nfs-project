#include "header.h"
#include "function_def.h"
#include "basic_header.h"
#include "Storage_semaphore.h"
#include "S_centre.h"
#include "Main_semaphore.h"
#include "macros.h"
#include "Path_ds.h"
#include "duplicacy.h"

sem_t mutex;

struct com
{
    int h;
    char arr[1024];
};

void cbuff(int fd)
{
    char a[1];
    a[0] = EOF;
    recv(fd, a, 1, MSG_DONTWAIT);
    while (a[0] != EOF)
    {
        printf("%s", a);
        a[0] = EOF;
        recv(fd, a, 1, MSG_DONTWAIT);
    }
    return;
}

void insert_rec1(char *a, int idx, int id, struct trie *tree)
{

    int cnt = getkey(a[idx]);
    struct trie *temp = tree->next[cnt];
    if (temp == NULL)
    {
        tree->next[cnt] = malloc(sizeof(struct trie));
        for (int i = 0; i < validwords; i++)
        {
            tree->next[cnt]->next[i] = NULL;
        }
    }
    if (a[idx + 1] == '\0' || a[idx + 1] == '\n')
    {
        tree->next[cnt]->word_status = IsWord;
        tree->next[cnt]->id = id;
        return;
    }
    insert_rec1(a, idx + 1, id, tree->next[cnt]);
    return;
}
int sockooo;
void *checkforclosing(void *arg)
{
    int port = ((struct entries *)arg)->port;
    int k = ((struct entries *)arg)->numberofpaths;
    char *ip = "127.0.0.1";
    struct sockaddr_in addr;
    sockooo = socket(AF_INET, SOCK_STREAM, 0);
    if (sockooo < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    int a = connect(sockooo, (struct sockaddr *)&addr, sizeof(addr));
    if (a == -1)
    {
        perror("connection unsuccessful back");
        return NULL;
    }
    int aa = 0;
    recv(sockooo, &aa, sizeof(int), 0);
    printf("Storage %d Server Disconnected\n", k);
    STORAGE_SERVER[k].status = WAITING;
    return NULL;
}

void *

handle_client(void *arg)
{
    int client_socket = *((int *)arg);
    struct entries inp;

    //// Acks started
    int number = 9;
    recv(client_socket, &number, 4, 0);
    number = 9;
    send(client_socket, &number, 4, 0);
    //// Acks end
    printf("Currently Here: 1\n");
    //// Details Recvd

    ssize_t bytes_received = recv(client_socket, &inp, sizeof(inp), 0);

    // if (bytes_received != sizeof(inp))
    // {
    //     printf("%ld %ld\n", bytes_received, sizeof(inp));
    //     perror("Error reading client request");
    //     close(client_socket);
    //     pthread_exit(NULL);
    // }

    for(int ii=0;ii<inp.numberofpaths;ii++)
    {
        printf("%s\n",inp.pathsaccessible[ii]);
    }

    printf("%ld %ld\n", bytes_received, sizeof(inp));
    printf("Details of Arrived Storage Server\n");
    printf("Port for Client: %d\nPort for back connection: %d\nNo. of accessible paths:%d One of the path:%s\n", inp.portforclient, inp.port, inp.numberofpaths,inp.pathsaccessible[1]);

    /// checking if ss already existed
    int id = check_old(inp.portforclient);

    /// finding a new empty id
    sem_wait(&mutex);
    int k = FIND_STORAGE_SERVER_ID();
    sem_post(&mutex);

    /// Checking for limits
    if (k == -1 && id == -1)
    {
        printf("Panic: Max SS Limit Reached\n");
        while (1)
            ;
    }

    if (id != -1)
    {
        printf("An old server Reconnected!!\n");

        k = id;
        STORAGE_SERVER[k].status = ACTIVE;

        /// Updating Copies, sice server reconnected; it may had gone through changes..

        char oo[256];
        snprintf(oo, 10, "./_%d", k);

        // deleting prev copies..
        if (STORAGE_SERVER[STORAGE_SERVER[k].dupli1].status == ACTIVE)
        {
            sem_wait(&global_mutex_empty[STORAGE_SERVER[k].dupli1]);
            sem_wait(&global_mutex_arr[STORAGE_SERVER[k].dupli1]);
            TODO[STORAGE_SERVER[k].dupli1].operation = DELETE_DIR;
            strcpy(TODO[STORAGE_SERVER[k].dupli1].path1, oo);
            sem_post(&global_mutex_full[STORAGE_SERVER[k].dupli1]);
            sem_post(&global_mutex_arr[STORAGE_SERVER[k].dupli1]);
            while (TODO[STORAGE_SERVER[k].dupli1].operation == DELETE_DIR)
                ;
        }
        if (STORAGE_SERVER[STORAGE_SERVER[k].dupli2].status == ACTIVE)
        {
            sem_wait(&global_mutex_empty[STORAGE_SERVER[k].dupli2]);
            sem_wait(&global_mutex_arr[STORAGE_SERVER[k].dupli2]);
            TODO[STORAGE_SERVER[k].dupli1].operation = DELETE_DIR;
            strcpy(TODO[STORAGE_SERVER[k].dupli2].path1, oo);
            sem_post(&global_mutex_full[STORAGE_SERVER[k].dupli2]);
            sem_post(&global_mutex_arr[STORAGE_SERVER[k].dupli2]);
            while (TODO[STORAGE_SERVER[k].dupli2].operation == DELETE_DIR)
                ;
        }

        // setting up flag so that, treated differently in duplicacy.c
        STORAGE_SERVER[k].dupyflag = -6969;
    }
    else
    {
        STORAGE_SERVER[k].status = ACTIVE;
        STORAGE_SERVER[k].client_port = inp.portforclient;
        STORAGE_SERVER[k].num_paths = inp.numberofpaths;
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        getpeername(client_socket, (struct sockaddr *)&client_addr, &addr_len);
        printf("IP COPYING: %s", inet_ntoa(client_addr.sin_addr));
        strcpy(STORAGE_SERVER[k].ip_address, inet_ntoa(client_addr.sin_addr));
        printf("A new Storage Server Connected\n");
    }

    inp.numberofpaths = k;
    pthread_t check;

    /// Making a back connection.
    pthread_create(&check, NULL, checkforclosing, (void *)&inp);
    sleep(1);
    printf("Back Connection Started\n");

    for (int i = 0; i < STORAGE_SERVER[k].num_paths; i++)
    {
        strcpy(STORAGE_SERVER[k].accessible_paths[i], inp.pathsaccessible[i]);
    }

    if (id == -1)
        Insert_Dir(inp.pathsaccessible, STORAGE_SERVER[k].num_paths, k);
    total_cnt_ss++;

    while (1)
    {

        /// Resetting buffers
        cbuff(client_socket);
        sem_wait(&global_mutex_full[k]);
        sem_wait(&global_mutex_arr[k]);



        printf("Requested operation %d on path1::%s and path2(if applicable)::%s on Storage Server::%d\n", TODO[k].operation, TODO[k].path1, TODO[k].path2, k);
        struct global_operation tosend;
        tosend.operation = TODO[k].operation;
        strcpy(tosend.path1, TODO[k].path1);
        strcpy(tosend.path2, TODO[k].path2);
        int l = find_pathid(TODO[k].path1, &STORAGE_SERVER[k]);
        if (TODO[k].operation >= 7)
        {
            char ack[5];
            if (TODO[k].operation == 8)
            {
                STORAGE_SERVER[k].flag_RW[l] = 0;
            }
            else
            {
                STORAGE_SERVER[k].flag_RW[l]--;
            }
            if (strcmp("OK", ack) != 0)
            {
                TODO[k].operation = -1;
            }
            else
            {
                TODO[k].operation = -2;
            }
            sem_post(&global_mutex_arr[k]);
            sem_post(&global_mutex_empty[k]);
            continue;
        }
        else
        {
            int s1 = send(client_socket, &tosend, sizeof(tosend), 0);
            if (s1 < 0)
            {
                perror("Error sending data");
            }
            if (TODO[k].operation == DELETE_FILE || TODO[k].operation == DELETE_DIR || TODO[k].operation == CREATE_DIR || TODO[k].operation == CREATE_FILE)
            {
                char ack1[5];
                int res = -1;
                int bs = recv(client_socket, &res, 4, 0);
                ack1[0] = '_';
                if (bs < 0)
                {
                    perror("Error recieving data");
                }
                else if (bs == 0)
                {
                    STORAGE_SERVER[k].status = WAITING;
                    sem_post(&global_mutex_arr[k]);
                    sem_post(&global_mutex_empty[k]);
                    return NULL;
                }
                if (res != -1)
                {
                    TODO[k].operation = -1;
                }
                else
                {
                    TODO[k].operation = -2;
                }
                sem_post(&global_mutex_arr[k]);
                sem_post(&global_mutex_empty[k]);
                continue;
            }
            else if (TODO[k].operation == SELFCOPY)
            {
                char ack[5];
                strcpy(ack, "OK");
                struct copysender h;
                while (1)
                {
                    int bs = recv(client_socket, &h, sizeof(struct copysender), 0);
                    if (bs < 0)
                    {
                        perror("Error recieving data");
                    }
                    else if (bs == 0)
                    {
                        STORAGE_SERVER[k].status = WAITING;
                        sem_post(&global_mutex_arr[k]);
                        sem_post(&global_mutex_empty[k]);
                        return NULL;
                    }
                    int s2 = send(client_socket, ack, 5, 0);
                    if (s2 < 0)
                    {
                        perror("Error sending data");
                    }
                    if (h.number == 1)
                    {
                        insert_rec1(h.data, 0, k, &Directory);
                    }
                    if (h.number == -100)
                    {
                        break;
                    }
                }
                TODO[k].operation = -1;
                sem_post(&global_mutex_arr[k]);
                sem_post(&global_mutex_empty[k]);
                continue;
            }
            else if (TODO[k].operation == COPY_DIR || TODO[k].operation == COPY_FILE || TODO[k].operation == -5)
            {
                cbuff(client_socket);
                int number = 6;
                recv(client_socket, &number, 4, 0);
                number = 6;
                send(client_socket, &number, 4, 0);
                int res = 0;
                struct copysender h; // global struct
                while (1)
                {
                    ssize_t bs = recv(client_socket, &h, sizeof(struct copysender), 0);
                    if (bs < 0)
                    {
                        perror("Error recieving data");
                    }
                    else if (bs == 0)
                    {
                        STORAGE_SERVER[k].status = WAITING;
                        sem_post(&global_mutex_arr[k]);
                        sem_post(&global_mutex_empty[k]);
                        return NULL;
                    }
                    int paste_id = other_id[k];
                    while (copy_data[paste_id].number != -69)
                        ;
                    copy_data[paste_id].type = h.type;
                    strcpy(copy_data[paste_id].data, h.data);
                    copy_data[paste_id].number = h.number;
                    while (copy_data[paste_id].number == h.number)
                        ;
                    int s3 = send(client_socket, &res, 4, 0);
                    if (s3 < 0)
                    {
                        perror("Error sending data");
                    }
                    if (h.number == -100)
                    {
                        break;
                    }
                }
                TODO[k].operation = -1;
                other_id[k] = -1;
                sem_post(&global_mutex_arr[k]);
                sem_post(&global_mutex_empty[k]);
                continue;
            }
            else if (TODO[k].operation == PASTE)
            {

                char ack[5];
                strcpy(ack, "OK");
                int flag = 0;
                int bs;
                while (1)
                {
                    while (copy_data[k].number == -69)
                        ;
                    if (copy_data[k].number == 1)
                    {
                        insert_rec1(copy_data[k].data, 0, k, &Directory);
                    }
                    int s4 = send(client_socket, &copy_data[k], sizeof(struct copysender), 0);
                    if (s4 < 0)
                    {
                        perror("Error sending data");
                    }
                    int res = -1;
                    bs = recv(client_socket, &res, 4, 0);
                    if (bs == 0)
                    {
                        copy_data[k].number = -69;
                        STORAGE_SERVER[k].status = WAITING;
                        sem_post(&global_mutex_arr[k]);
                        sem_post(&global_mutex_empty[k]);
                        return NULL;
                    }
                    if (copy_data[k].number == -100)
                    {
                        flag = 1;
                    }
                    copy_data[k].number = -69;
                    if (flag == 1)
                        break;
                }
                spinlock--;
                TODO[k].operation = -1;
                sem_post(&global_mutex_arr[k]);
                sem_post(&global_mutex_empty[k]);
                continue;
            }
        }
    }

    return NULL;
}
