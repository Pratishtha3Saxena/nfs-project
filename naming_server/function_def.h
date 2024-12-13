// main
void initialise_naming_server(void);
void set_initial_values_nm(void);
void* set_up_tcp_request(void*);
void intialize_central_storage_repo(void);
void* set_up_dupy(void *);


// Storage_data
void *handle_client(void *arg);


// user data
void *handle_user(void *arg);
void* set_up_tcp_user(void *);
int Find_Path(char* a);



//Scentre
char* print_paths(void);
int FIND_STORAGE_SERVER_ID(void);
void intialize_global_mutex(void);
int check_old(int port);
int find_pathid(char a[256], struct request_header* r);


// DS 

void Insert_Dir(char a[200][256], int path_num, int id);
void intialise_Directory(void);
int getkey(char a);


// void Insert_Dir(char **a, int path_num, int id);


//lrU


void intialize_cache(void);
int check_LRUa(int operation,char buffer1[256]);
int check_LRUb(int operation,char buffer1[256],char buffer2[256]);
int check_LRU(char buffer1[256]);


void insert_LRU(char buffer[256], int port);
void delete_LRU(char buffer[256]);


//redundancy 
int* redundancy(int ownindex);


