#include "basic_header.h"
#include "header.h"
#include "function_def.h"
#include "duplicacy.h"
#include "macros.h"
#include "S_centre.h"

int total_cnt_ss;

int spinlock = 0;

int tocopy(char *a)
{
    int ans = 0;
    if (a[2] == '_' || a[2] == '.' || (a[2] == 'S' && a[3] == 'S' && a[4] == '\0'))
        return 0;
    for (size_t i = 0; i < strlen(a); i++)
    {
        if (a[i] == '/')
            ans++;
        if (ans > 1)
            break;
    }
    if (ans == 1)
        return 1;
    else
        return 0;
}

void *set_up_dupy(void *)
{
    // GOGO;
    while (total_cnt_ss < 3)
        ;
    int local_cnt = 0;
    while (1)
    {
        GOGO;
        while (local_cnt == total_cnt_ss)
            ;
        for (int i = 0; i < Max_SS_no; i++)
        {
            if (STORAGE_SERVER[i].status == EMPTY)
                break;
            for (int kk = 0; kk < Max_SS_no; kk++)
                copy_data[kk].number = -69;
            if (STORAGE_SERVER[i].status != EMPTY && STORAGE_SERVER[i].dupli1 == -1)
            {
                printf("COPY MAKING TIME!!\n");
                spinlock += 2;
                while (other_id[i] != -1)
                    ;
                // GOGO;
                // THERE;
                int *a = redundancy(i);
                int one = a[0];
                int two = a[1];
                char desti[256] = "./_";
                char desti1[10];
                sprintf(desti1, "%d", i);
                strcat(desti, desti1);
                sem_wait(&global_mutex_empty[one]);
                sem_wait(&global_mutex_arr[one]);
                TODO[one].operation = CREATE_DIR;
                // GOGO;
                strcpy(TODO[one].path1, desti);
                sem_post(&global_mutex_arr[one]);
                sem_post(&global_mutex_full[one]);
                sem_wait(&global_mutex_empty[two]);
                sem_wait(&global_mutex_arr[two]);
                TODO[two].operation = CREATE_DIR;
                strcpy(TODO[two].path1, desti);
                sem_post(&global_mutex_arr[two]);
                sem_post(&global_mutex_full[two]);

                STORAGE_SERVER[i].dupli1 = one;
                STORAGE_SERVER[i].dupli2 = two;
                while (TODO[two].operation == CREATE_DIR || TODO[one].operation == CREATE_DIR)
                    ;

                for (int j = 0; j < STORAGE_SERVER[i].num_paths; j++)
                {
                    while (other_id[i] != -1)
                        ;
                    sem_wait(&global_mutex_empty[i]);
                    sem_wait(&global_mutex_empty[one]);
                    sem_wait(&global_mutex_arr[i]);
                    sem_wait(&global_mutex_arr[one]);
                    other_id[i] = one;
                    TODO[i].operation = -5;
                    strcpy(TODO[i].path1, STORAGE_SERVER[i].accessible_paths[j]);
                    strcpy(TODO[i].path2, desti);
                    TODO[one].operation = PASTE;
                    sem_post(&global_mutex_arr[one]);
                    sem_post(&global_mutex_arr[i]);
                    sem_post(&global_mutex_full[one]);
                    sem_post(&global_mutex_full[i]);
                }
                for (int j = 0; j < STORAGE_SERVER[i].num_paths; j++)
                {
                    while (other_id[i] != -1)
                        ;

                    sem_wait(&global_mutex_empty[i]);
                    sem_wait(&global_mutex_empty[two]);
                    sem_wait(&global_mutex_arr[i]);
                    sem_wait(&global_mutex_arr[two]);
                    other_id[i] = two; 
                    TODO[i].operation = -5;
                    strcpy(TODO[i].path1, STORAGE_SERVER[i].accessible_paths[j]);
                    strcpy(TODO[i].path2, desti);
                    TODO[two].operation = PASTE;
                    sem_post(&global_mutex_arr[two]);
                    sem_post(&global_mutex_arr[i]);
                    sem_post(&global_mutex_full[two]);
                    sem_post(&global_mutex_full[i]);
                }
            }
            if (STORAGE_SERVER[i].status != EMPTY && STORAGE_SERVER[i].dupyflag == -6969)
            {
                int one = STORAGE_SERVER[i].dupli1;
                int two = STORAGE_SERVER[i].dupli2;
                char desti[256] = "./";
                char desti1[10];

                sprintf(desti1, "%d", i);
                strcat(desti, desti1);
                if (STORAGE_SERVER[one].status == ACTIVE)
                {
                    sem_wait(&global_mutex_empty[one]);
                    sem_wait(&global_mutex_arr[one]);
                    TODO[one].operation = CREATE_DIR;
                    strcpy(TODO[one].path1, desti);
                    sem_post(&global_mutex_arr[one]);
                    sem_post(&global_mutex_full[one]);
                    sem_wait(&global_mutex_empty[two]);
                    sem_wait(&global_mutex_arr[two]);
                    TODO[two].operation = CREATE_DIR;
                    strcpy(TODO[two].path1, desti);
                    sem_post(&global_mutex_arr[two]);
                    sem_post(&global_mutex_full[two]);

                    STORAGE_SERVER[i].dupli1 = one;
                    STORAGE_SERVER[i].dupli2 = two;
                    while (TODO[two].operation == CREATE_DIR || TODO[one].operation == CREATE_DIR)
                        ;

                    for (int j = 0; j < STORAGE_SERVER[i].num_paths; j++)
                    {
                        while (other_id[i] != -1)
                            ;
                        sem_wait(&global_mutex_empty[i]);

                        sem_wait(&global_mutex_empty[one]);
                        sem_wait(&global_mutex_arr[i]);
                        sem_wait(&global_mutex_arr[one]);
                        other_id[i] = one;
                        TODO[i].operation = -5;
                        strcpy(TODO[i].path1, STORAGE_SERVER[i].accessible_paths[j]);
                        strcpy(TODO[i].path2, desti);
                        TODO[one].operation = PASTE;
                        sem_post(&global_mutex_arr[one]);
                        sem_post(&global_mutex_arr[i]);
                        sem_post(&global_mutex_full[one]);
                        sem_post(&global_mutex_full[i]);

                    }
                }

                if (STORAGE_SERVER[two].status == ACTIVE)
                {

                    for (int j = 0; j < STORAGE_SERVER[i].num_paths; j++)
                    {
                        while (other_id[i] != -1)
                            ;
                        sem_wait(&global_mutex_empty[i]);

                        sem_wait(&global_mutex_empty[two]);
                        sem_wait(&global_mutex_arr[i]);
                        sem_wait(&global_mutex_arr[two]);
                        other_id[i] = two; // if (tocopy(STORAGE_SERVER[i].accessible_paths[j]) == 0)
                        TODO[i].operation = -5;
                        strcpy(TODO[i].path1, STORAGE_SERVER[i].accessible_paths[j]);
                        strcpy(TODO[i].path2, desti);
                        TODO[two].operation = PASTE;
                        sem_post(&global_mutex_arr[two]);
                        sem_post(&global_mutex_arr[i]);
                        sem_post(&global_mutex_full[two]);
                        sem_post(&global_mutex_full[i]);
                    }
                }
            }
            local_cnt = total_cnt_ss;
        }
    }
}