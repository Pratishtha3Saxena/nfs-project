


typedef struct cell
{
    char path1[256];
    struct cell *nextnode;
    struct cell *prevnode;
    int port;

} cell;

typedef struct LRU
{
    cell *start;
    int no_of_cells;
}LRU;


extern struct LRU *cache;
