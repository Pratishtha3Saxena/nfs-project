#define Max_SS_no 32
#define EMPTY 0
#define ACTIVE 1
#define WAITING 2
#define MAX_PORT_DEFINE 65000

struct copysender
{
    int number;
    char data[1024];
    int type;
};



extern struct request_header STORAGE_SERVER[Max_SS_no];
extern int backupcount[Max_SS_no];
extern int id_tracker[MAX_PORT_DEFINE];

extern struct global_operation TODO[Max_SS_no];
extern sem_t global_mutex_full[Max_SS_no];
extern sem_t global_mutex_empty[Max_SS_no];
extern sem_t global_mutex_arr[Max_SS_no];
extern int backupcount[Max_SS_no];
extern int READER_WRITER[Max_SS_no];
extern sem_t mut;
extern int other_id[Max_SS_no];
extern struct copysender copy_data[Max_SS_no];


