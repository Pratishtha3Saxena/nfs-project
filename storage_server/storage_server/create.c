#include "basic_header.h"
int createfile(char *path, int number)
{

    // check if it is to create a file or folder
    if (number == 0)
    {
        int a = mkdir(path, 0777);
        if (a != 0)
        {
            return -1;
        }
        else {
            return 0;

        }
    }
    else
    {
        // printf("%s\n",path);
        int fd = open(path, O_CREAT, 0777);
        if (fd == -1)
        {
            printf("no\n");
            return -1;
        }
        close(fd);
        return 0;
    }
}
