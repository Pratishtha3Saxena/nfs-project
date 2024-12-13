#ifndef _SSH_
#define _SSH_

#include"macros.h"

struct entries {
    int port;
    int portforclient;
    int numberofpaths;
    char pathsaccessible[MAX_PATHS][MAX_PATH_LENGTH];
    char ip[16];
};

struct copysender
{
    int number;
    char data[MAX_BUFFER_AT_ONE_TIME];
    int type;
};

struct firstdata{
    int a;
    int b;
};
struct com
{
    int h;
    char arr[1024];
    int socket;
    char arr1[1024];
};
typedef struct firstdata firstdata;
typedef firstdata* ptrtofirstdata;
typedef struct entries entries;
typedef entries* ptrtoentries;

typedef struct copysender copysender;
typedef copysender *ptrtocopysender;

extern entries entry;

#endif