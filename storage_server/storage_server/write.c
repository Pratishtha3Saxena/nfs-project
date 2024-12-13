#include "basic_header.h"
#include "macros.h"
int writedata(char *data,int fd)
{
    ssize_t w = write(fd,data,strlen(data));
    if(w < 0)
    {
        perror("Error in writing");
        return -1;
    }
    // printf("%ld\n",w);
    memset(data, '\0', MAX_BUFFER_AT_ONE_TIME);
    return 0;
}