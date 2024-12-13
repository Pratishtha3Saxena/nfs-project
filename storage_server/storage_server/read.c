#include "basic_header.h"
#include "macros.h"
int readdata(char *data,int fd)
{
    int a = read(fd, data, MAX_BUFFER_AT_ONE_TIME-1);
    data[a]='\0';
    if(a < 0)
    {
        perror("Error in reading file");
    }
    return a;
}