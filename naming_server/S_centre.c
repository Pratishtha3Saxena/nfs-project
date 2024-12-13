#include "basic_header.h"
#include "header.h"
#include "macros.h"
#include "S_centre.h"
#include "function_def.h"
#include "duplicacy.h"


struct request_header STORAGE_SERVER[Max_SS_no];
int other_id[Max_SS_no];
struct copysender copy_data[Max_SS_no];

struct global_operation TODO[Max_SS_no];
int READER_WRITER[Max_SS_no];

sem_t global_mutex_full[Max_SS_no];
sem_t global_mutex_empty[Max_SS_no];
sem_t global_mutex_arr[Max_SS_no];



int id_tracker[MAX_PORT_DEFINE];


void intialize_global_mutex(void)
{
    for(int i=0;i<Max_SS_no;i++)
    {
        sem_init(&global_mutex_full[i],0,0);
        sem_init(&global_mutex_empty[i],0,1);
        sem_init(&global_mutex_arr[i],0,1);
        
    }
}


void intialize_central_storage_repo(void)
{
    total_cnt_ss=0;
    for(int i=0;i<Max_SS_no;i++)
    {
        STORAGE_SERVER[i].status=EMPTY;
        backupcount[i]=0;
        other_id[i]=-1;
        copy_data[i].number=-69;
        READER_WRITER[i]=0;
        STORAGE_SERVER[i].dupli1=-1;
        for(int j=0;j<MAX_PATHS;j++)
        STORAGE_SERVER[i].flag_RW[j]=0;
    }

    sem_init(&mut,0,1);

}


int FIND_STORAGE_SERVER_ID(void)
{
    for(int i=0;i<Max_SS_no;i++)
    {
        if(STORAGE_SERVER[i].status==EMPTY)
        {
            return i;
        }
    }
    return -1;
}

int check_old(int port)
{
    for(int i=0;i<Max_SS_no;i++)
    {
        if(STORAGE_SERVER[i].client_port==port && STORAGE_SERVER[i].status!=EMPTY)
        {
            return i;
        }
    }
    return -1;
}

int find_pathid(char a[256], struct request_header* r)
{
    for(int i=0;i<MAX_PATHS;i++)
    {
        if(strcmp(r->accessible_paths[i],a)==0)
        {
            return i;
        }
    }
    return -1;
}

char* print_paths(void) {
    char* paths = malloc(8192);
    memset(paths, '\0', 8192);
    for(int i=0;i<Max_SS_no;i++)
    {
        for (int j = 0; j < STORAGE_SERVER[i].num_paths; j++) {
            strcat(paths, STORAGE_SERVER[i].accessible_paths[j]);
            strcat(paths, "\n");
        }
    }
    return paths;
}
