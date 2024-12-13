#include "basic_header.h"
#include "Main_semaphore.h"
#include "Path_ds.h"
#include "LRU.h"
#include "header.h"
#include "S_centre.h"
#include "function_def.h"

struct trie Directory;
struct LRU *cache;
int backupcount[Max_SS_no];
sem_t mut;
sem_t main_s;
sem_t user_s;
pthread_t storage_req_thread;
pthread_t client_req_thread;
pthread_t duplicacy_req_thread;

int main()
{
    // Basic Initialization...
    printf("Starting Naming Server...\n");


    sem_init(&main_s, 0, 1);
    intialize_central_storage_repo();
    intialize_global_mutex();
    intialize_cache();

    // Threading...

    pthread_create(&storage_req_thread, NULL, set_up_tcp_request, NULL);
    pthread_create(&client_req_thread, NULL, set_up_tcp_user, NULL);
    pthread_create(&duplicacy_req_thread, NULL, set_up_dupy, NULL);
    pthread_join(storage_req_thread, NULL);
}