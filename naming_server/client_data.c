#include "basic_header.h"
#include "header.h"
#include "Storage_semaphore.h"
#include "S_centre.h"
#include "Main_semaphore.h"
#include "macros.h"
#include "client.h"
#include "Path_ds.h"
#include "function_def.h"

int delete_rec_path1(char a[256], int idx, struct trie *tree)
{
    int cnt = getkey(a[idx]);
    struct trie *temp = tree->next[cnt];

    if (tree->next[cnt] == NULL)
    {
        return -1;
    }
    if (a[idx + 1] == '\0' || a[idx + 1] == '\n')
    {
        if (temp->word_status == 1)
        {
            temp->word_status = 0;
            return 0;
        }
        else
            return -1;
    }
    return delete_rec_path1(a, idx + 1, temp);
}

void insert_rec2(char *a, int idx, int id, struct trie *tree)
{

    int cnt = getkey(a[idx]);
    struct trie *temp = tree->next[cnt];
    if (temp == NULL) // if (cnt <= 0)
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
    insert_rec2(a, idx + 1, id, tree->next[cnt]);
    return;
}

void *handle_user(void *arg)
{
    printf("A NEW CLIENT ARRIVED\n");
    int client_socket = *((int *)arg);
    char buffer[8192];
    struct client_req first_req;

    int bytes_received = recv(client_socket, &first_req, sizeof(first_req), 0);
    if (bytes_received != sizeof(first_req))
    {
        perror("Error reading client request");
        close(client_socket);
        pthread_exit(NULL);
    }
    printf("Operation Requested:%d on path1:%s and path2(if applicable)::%s\n", first_req.number, first_req.path1, first_req.path2);
    int k;
    struct client_send sent;
    if (first_req.number == 15)
    {
        memset(buffer,0,sizeof(buffer));
        char* temp = print_paths();
        strncpy(buffer, temp, 8192);

        sent.number = 2;
        int ok;
        printf("Here- -1 currently\n");

        send(client_socket, &sent, sizeof(sent), 0);
        printf("Here-0 currently\n");
        send(client_socket, buffer, sizeof(buffer), 0);
        printf("%s\n", buffer);
        printf("Here-1 currently\n");
        close(client_socket);
        return NULL;
    }
    else if (first_req.number != 5 && first_req.number != 6)
    {
        int x = check_LRU(first_req.path1);
        if (x != -1)
        {
            printf("HIT at SS:%d\n", x + 1);
            k = x;
        }
        else
        {
            if (first_req.number == 1 || first_req.number == 2)
            {
                char dummy[4096];
                strcpy(dummy, first_req.path1);
                int s = strlen(first_req.path1);
                int len = strlen(first_req.path1);
                for (int i = len - 1; i > 0; i--)
                {
                    if (first_req.path1[i] == '/')
                    {
                        first_req.path1[i] = '\0';
                        s = i;
                        break;
                    }
                }
                k = Find_Path(first_req.path1);
                if (k != -1) {
                    int kk;
                    // for (int j = 0; j < MAX_PATHS; j++) {
                    //     if (!STORAGE_SERVER[k].accessible_paths[j]) {
                    //         kk = j;
                    //     }
                    // }
                    strcpy(STORAGE_SERVER[k].accessible_paths[STORAGE_SERVER[k].num_paths++], dummy);
                }
                first_req.path1[s] = '/';
                // free(dummy);
            }
            else
                k = Find_Path(first_req.path1);
        }
        if (x == -1)
        {
            insert_LRU(first_req.path1, k);
        }
        if (k == -1)
        {
            strcpy(buffer, "THE GIVEN PATH IS NOT YET REGISTERED WITH US, SORRY FOR INCONVENIENCE!");
            printf("%s\n", buffer);
            buffer[71] = '\0';
            sent.number = 1;
            int ok;
            send(client_socket, &sent, sizeof(sent), 0);
            recv(client_socket, &ok, 4, 0);
            send(client_socket, buffer, sizeof(buffer), 0);
            close(client_socket);
            return NULL;
        }
        int og = k;
        if (STORAGE_SERVER[k].status == WAITING)
        {
            k = STORAGE_SERVER[k].dupli1;
            if (first_req.number != READ)
            {
                strcpy(buffer, "Only Read allowed at the moment");
                printf("%s\n", buffer);
                buffer[71] = '\0';
                sent.number = 1;
                int ok;
                send(client_socket, &sent, sizeof(sent), 0);
                recv(client_socket, &ok, 4, 0);
                send(client_socket, buffer, sizeof(buffer), 0);
                close(client_socket);
                return NULL;
            }
        }
        if (STORAGE_SERVER[k].status == WAITING)
            k = STORAGE_SERVER[k].dupli2;
        if (STORAGE_SERVER[k].status == WAITING)
        {
            strcpy(buffer, "Bad Luck!, The server and its backups, all are currenty down");
            printf("%s\n", buffer);
            buffer[71] = '\0';
            sent.number = 1;
            int ok;
            send(client_socket, &sent, sizeof(sent), 0);
            recv(client_socket, &ok, 4, 0);
            send(client_socket, buffer, sizeof(buffer), 0);
            close(client_socket);
            return NULL;
        }
        int ll = find_pathid(first_req.path1, &STORAGE_SERVER[og]);
        int l = STORAGE_SERVER[og].flag_RW[ll];

        if (first_req.number == 8)
        {

            if (l != 0)
            {

                int ok;
                sent.number = 2;
                send(client_socket, &sent, sizeof(sent), 0);
                strcpy(buffer, "Sorry Reading Going on, Better LUCK next time");
                printf("%s\n", buffer);
                recv(client_socket, &ok, 4, 0);
                send(client_socket, buffer, sizeof(buffer), 0);
                close(client_socket);
                return NULL;
            }
        }
        else if (first_req.number >= 7)
        {
            if (l < 0)
            {
                int ok;
                sent.number = 2;
                send(client_socket, &sent, sizeof(sent), 0);
                strcpy(buffer, "Sorry Writing Going on, Better LUCK next time");
                recv(client_socket, &ok, 4, 0);
                send(client_socket, buffer, sizeof(buffer), 0);
                printf("%s\n", buffer);
                close(client_socket);
                return NULL;
            }
        }
        if (first_req.number >= 7)
        {
            if (first_req.number == 8)
            {
                STORAGE_SERVER[k].flag_RW[ll] = -1;
            }
            else
            {
                STORAGE_SERVER[k].flag_RW[ll]++;
            }
            if (og != k)
                sent.number = og + 10 * 3;
            else
                sent.number = 0;

            sent.port = STORAGE_SERVER[k].client_port;
            strcpy(sent.ip, STORAGE_SERVER[k].ip_address);
            printf("IP GOING: %s\n", sent.ip);
            send(client_socket, &sent, sizeof(sent), 0);
            char ack[5];
            recv(client_socket, ack, 5, 0);
            if (first_req.number == 8)
                STORAGE_SERVER[k].flag_RW[ll] = 0;
            else
                STORAGE_SERVER[k].flag_RW[ll]--;
        }
        if (first_req.number >= 7)
        {
            if (STORAGE_SERVER[k].dupli1 != -1 && first_req.number == WRITE)
            {
                if (STORAGE_SERVER[STORAGE_SERVER[k].dupli1].status == ACTIVE)
                {
                    sem_wait(&global_mutex_empty[STORAGE_SERVER[k].dupli1]);
                    sem_wait(&global_mutex_arr[STORAGE_SERVER[k].dupli1]);
                    printf("Maintaing The Copy !! at SS %d of %d\n", STORAGE_SERVER[k].dupli1, k);
                    TODO[STORAGE_SERVER[k].dupli1].operation = DELETE_FILE;
                    char oo[256];
                    snprintf(oo, 256, "./_%d/", k);
                    int aa = 0;
                    int bb;
                    while (1)
                    {
                        if (oo[aa] != '\0')
                            aa++;
                        else
                        {
                            bb = aa;
                            for (size_t i = 2; i < strlen(first_req.path1); i++)
                            {
                                if (first_req.path1[i] == '/')
                                {
                                    aa = bb;
                                    continue;
                                }
                                oo[aa++] = first_req.path1[i];
                            }
                            oo[aa++] = '\0';
                            break;
                        }
                    }

                    printf("DUPY PATH = %s\n", oo);
                    strcpy(TODO[STORAGE_SERVER[k].dupli1].path1, oo);
                    sem_post(&global_mutex_arr[STORAGE_SERVER[k].dupli1]);
                    sem_post(&global_mutex_full[STORAGE_SERVER[k].dupli1]);
                    while (TODO[STORAGE_SERVER[k].dupli1].operation == DELETE_FILE)
                        ;
                    // HERE;
                    ////
                    sem_wait(&global_mutex_empty[k]);
                    sem_wait(&global_mutex_empty[STORAGE_SERVER[k].dupli1]);
                    sem_wait(&global_mutex_arr[k]);
                    sem_wait(&global_mutex_arr[STORAGE_SERVER[k].dupli1]);

                    other_id[k] = STORAGE_SERVER[k].dupli1;
                    TODO[k].operation = -5;
                    snprintf(oo, 256, "./_%d", k);
                    aa = 0;

                    printf("Copy path to rewrite: %s\n", oo);
                    strcpy(TODO[k].path2, oo);
                    strcpy(TODO[k].path1, first_req.path1);
                    TODO[STORAGE_SERVER[k].dupli1].operation = PASTE;
                    sem_post(&global_mutex_full[k]);
                    sem_post(&global_mutex_arr[k]);
                    sem_post(&global_mutex_full[STORAGE_SERVER[k].dupli1]);
                    sem_post(&global_mutex_arr[STORAGE_SERVER[k].dupli1]);
                    while (TODO[k].operation == COPY_FILE || TODO[STORAGE_SERVER[k].dupli1].operation == PASTE)
                        ;
                    THERE;
                }

                /////////////////
                if (STORAGE_SERVER[STORAGE_SERVER[k].dupli2].status == ACTIVE)
                {
                    char oo[256];
                    sem_wait(&global_mutex_empty[STORAGE_SERVER[k].dupli2]);
                    sem_wait(&global_mutex_arr[STORAGE_SERVER[k].dupli2]);
                    printf("Maintaing The Copy !! at SS %d of %d\n", STORAGE_SERVER[k].dupli2, k);
                    TODO[STORAGE_SERVER[k].dupli2].operation = DELETE_FILE;
                    snprintf(oo, 256, "./_%d/", k);
                    int aa = 0;
                    int bb;
                    while (1)
                    {
                        if (oo[aa] != '\0')
                            aa++;
                        else
                        {
                            bb = aa;
                            for (size_t i = 2; i < strlen(first_req.path1); i++)
                            {
                                if (first_req.path1[i] == '/')
                                {
                                    aa = bb;
                                    continue;
                                }
                                oo[aa++] = first_req.path1[i];
                            }
                            oo[aa++] = '\0';
                            break;
                        }
                    }

                    printf("DUPY PATH = %s\n", oo);
                    strcpy(TODO[STORAGE_SERVER[k].dupli2].path1, oo);
                    sem_post(&global_mutex_arr[STORAGE_SERVER[k].dupli2]);
                    sem_post(&global_mutex_full[STORAGE_SERVER[k].dupli2]);
                    while (TODO[STORAGE_SERVER[k].dupli2].operation == DELETE_FILE)
                        ;
                    // HERE;
                    ////
                    sem_wait(&global_mutex_empty[k]);
                    sem_wait(&global_mutex_empty[STORAGE_SERVER[k].dupli2]);
                    sem_wait(&global_mutex_arr[k]);
                    sem_wait(&global_mutex_arr[STORAGE_SERVER[k].dupli2]);

                    other_id[k] = STORAGE_SERVER[k].dupli2;
                    TODO[k].operation = -5;
                    snprintf(oo, 256, "./_%d", k);
                    aa = 0;

                    printf("Copy path to rewrite: %s\n", oo);
                    strcpy(TODO[k].path2, oo);
                    strcpy(TODO[k].path1, first_req.path1);
                    TODO[STORAGE_SERVER[k].dupli2].operation = PASTE;
                    sem_post(&global_mutex_full[k]);
                    sem_post(&global_mutex_arr[k]);
                    sem_post(&global_mutex_full[STORAGE_SERVER[k].dupli2]);
                    sem_post(&global_mutex_arr[STORAGE_SERVER[k].dupli2]);
                    while (TODO[k].operation == COPY_FILE || TODO[STORAGE_SERVER[k].dupli2].operation == PASTE)
                        ;
                }
            }

            close(client_socket);
            return NULL;
        }
        else
        {
            sem_wait(&global_mutex_empty[k]);
            sem_wait(&global_mutex_arr[k]);
            if (first_req.number == CREATE_FILE || first_req.number == CREATE_DIR)
            {
                insert_rec2(first_req.path1, 0, k, &Directory);
            }
            if (first_req.number == 3 || first_req.number == 4)
            {
                delete_rec_path1(first_req.path1, 0, &Directory);
            }
            sent.number = 1;
            int ok;
            TODO[k].operation = first_req.number;
            strcpy(TODO[k].path1, first_req.path1);
            send(client_socket, &sent, sizeof(sent), 0);
            recv(client_socket, &ok, 4, 0);
            sem_post(&global_mutex_arr[k]);
            sem_post(&global_mutex_full[k]);
            while (TODO[k].operation == first_req.number)
                ;
            if (TODO[k].operation == -1)
                strcpy(buffer, "THE COMMAND SUCCESSFULLY PERFORMED");
            else
                strcpy(buffer, "THE COMMAND WAS NOT PERFORMED DUE TO SOME TECHNICAL ERROR, BETTER LUCK NEXT TIME");
            printf("%s\n", buffer);
            send(client_socket, buffer, sizeof(buffer), 0);
            close(client_socket);
            return NULL;
        }
    }
    else
    {
        int copy_path = check_LRU(first_req.path1);
        if (copy_path == -1)
        {
            copy_path = Find_Path(first_req.path1);
            insert_LRU(first_req.path1, copy_path);
        }
        int paste_path = check_LRU(first_req.path2);
        if (paste_path == -1)
        {
            paste_path = Find_Path(first_req.path2);
            insert_LRU(first_req.path2, paste_path);
        }
        sent.number = 1;
        int ok;
        send(client_socket, &sent, sizeof(sent), 0);
        recv(client_socket, &ok, 4, 0);
        if (copy_path == -1 || paste_path == -1)
        {
            strcpy(buffer, "THE GIVEN PATH IS NOT YET REGISTERED WITH US, SORRY FOR INCONVENIENCE!");
            buffer[71] = '\0';
            send(client_socket, buffer, sizeof(buffer), 0);
            close(client_socket);
            return NULL;
        }
        else
        {
            if (copy_path == paste_path)
            {
                sem_wait(&global_mutex_empty[copy_path]);
                sem_wait(&global_mutex_arr[copy_path]);
                TODO[copy_path].operation = SELFCOPY;
                strcpy(TODO[copy_path].path1, first_req.path1);
                strcpy(TODO[copy_path].path2, first_req.path2);
                sem_post(&global_mutex_arr[copy_path]);
                sem_post(&global_mutex_full[copy_path]);
                while (TODO[copy_path].operation == SELFCOPY)
                    ;
                if (TODO[copy_path].operation == -1)
                    strcpy(buffer, "THE COMMAND SUCCESSFULLY PERFORMED");
                else
                    strcpy(buffer, "THE COMMAND WAS NOT PERFORMED DUE TO SOME TECHNICAL ERROR, BETTER LUCK NEXT TIME");
                other_id[copy_path] = -1;
                send(client_socket, buffer, sizeof(buffer), 0);
            }
            else
            {
                sem_wait(&global_mutex_empty[copy_path]);

                sem_wait(&global_mutex_empty[paste_path]);
                sem_wait(&global_mutex_arr[copy_path]);
                sem_wait(&global_mutex_arr[paste_path]);
                other_id[copy_path] = paste_path;
                TODO[copy_path].operation = first_req.number;
                strcpy(TODO[copy_path].path1, first_req.path1);
                strcpy(TODO[copy_path].path2, first_req.path2);
                TODO[paste_path].operation = PASTE;
                sem_post(&global_mutex_arr[copy_path]);
                sem_post(&global_mutex_arr[paste_path]);
                sem_post(&global_mutex_full[copy_path]);
                sem_post(&global_mutex_full[paste_path]);
                while (TODO[copy_path].operation == first_req.number)
                    ;
                if (TODO[copy_path].operation == -1)
                    strcpy(buffer, "THE COMMAND SUCCESSFULLY PERFORMED");
                else
                    strcpy(buffer, "THE COMMAND WAS NOT PERFORMED DUE TO SOME TECHNICAL ERROR, BETTER LUCK NEXT TIME");
                other_id[copy_path] = -1;

                send(client_socket, buffer, sizeof(buffer), 0);
            }

            close(client_socket);
            return NULL;
        }
    }
}