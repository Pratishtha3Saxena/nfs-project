#include "basic_header.h"
#include "header.h"
#include "function_def.h"
#include "macros.h"
#include "LRU.h"

void intialize_cache(void)
{
    cache = malloc(sizeof(LRU));
    cache->start = NULL;
    cache->no_of_cells = 0;
    return ;
}


void delete_LRU(char buffer[256])
{
    
    cell* temp=cache->start;
    cell* ans;
    while(temp && strcmp(temp->path1,buffer)==0)
    {
        cache->start=temp->nextnode;
        if(temp->nextnode)
        temp->nextnode->prevnode=NULL;
        cache->no_of_cells--;
        free(temp);
        temp = cache->start;
    }
    while(temp && temp->nextnode!=NULL)
    {
        if(strcmp(temp->nextnode->path1,buffer)==0)
        {
            ans=temp->nextnode;
            temp->nextnode=temp->nextnode->nextnode;
            if(temp->nextnode->nextnode)
            temp->nextnode->nextnode->prevnode=temp;
            cache->no_of_cells--;
            free(ans);
            
        }
        else
        temp=temp->nextnode;
    }
    return ;
}


int check_LRU(char buffer1[256])
{
    cell *temp = cache->start;
    int ans_port;
    int idx = -1;
    for (int i = 0; i < cache->no_of_cells; i++)
    {
        if (temp && strcmp(temp->path1, buffer1) == 0 )
        {
            idx = i;
            ans_port = temp->port;
            if (i && temp->prevnode)
                temp->prevnode->nextnode = temp->nextnode;
            break;
        }
        else if (temp && i != cache->no_of_cells - 1)
        {
            temp = temp->nextnode;
        }
    }
    if (idx > 0 && temp)
    {
        printf("Cache Hit!\n");
        temp->nextnode = cache->start;
        cache->start = temp;
        return ans_port;
    }
    else if (idx == 0)
    {
        printf("Cache Hit!\n");
        return ans_port;
    }
    printf("CACHE MISS!\n");
    return -1;
}

void insert_LRU(char buffer[256], int port)
{
    if (cache->start == NULL)
    {
        cell *temp = malloc(sizeof(cell));
        temp->port = port;
        strcpy(temp->path1, buffer);
        temp->prevnode = NULL;
        temp->nextnode = NULL;
        cache->no_of_cells++;
        cache->start = temp;
        return;
    }
    else if (cache->no_of_cells < CACHE_LEN)
    {
        cell *temp = cache->start;
        cell *dummy = malloc(sizeof(cell));
        dummy->nextnode = temp;
        temp->prevnode = dummy;
        dummy->port = port;
        strcpy(dummy->path1, buffer);
        dummy->prevnode = NULL;
        cache->start = dummy;
        cache->no_of_cells++;
        return;
    }
    else
    {
        cell *temp = cache->start;
        while (temp->nextnode != NULL)
            temp = temp->nextnode;
        temp->prevnode->nextnode = NULL;
        temp->port = port;
        strcpy(temp->path1, buffer);
        temp->prevnode = NULL;
        temp->nextnode = cache->start;
        cache->start = temp;
        return;
    }
}