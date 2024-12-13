#include "basic_header.h"
#include "macros.h"
void additionalinfo(char *path, char *data)
{
    struct stat st;
    if (stat(path, &st) == 0)
    {
        memset(data, '\0', MAX_BUFFER_AT_ONE_TIME);
        sprintf(data, "%ld %ld ", st.st_size, st.st_mtime);
        char permissions[10];
        permissions[0] = (st.st_mode & S_IRUSR) ? 'r' : '-';
        permissions[1] = (st.st_mode & S_IWUSR) ? 'w' : '-';
        permissions[2] = (st.st_mode & S_IXUSR) ? 'x' : '-';
        permissions[3] = (st.st_mode & S_IRGRP) ? 'r' : '-';
        permissions[4] = (st.st_mode & S_IWGRP) ? 'w' : '-';
        permissions[5] = (st.st_mode & S_IXGRP) ? 'x' : '-';
        permissions[6] = (st.st_mode & S_IROTH) ? 'r' : '-';
        permissions[7] = (st.st_mode & S_IWOTH) ? 'w' : '-';
        permissions[8] = (st.st_mode & S_IXOTH) ? 'x' : '-';
        permissions[9] = '\0';

        strcat(data, permissions);
    }
    else
    {
        perror("invalid file or error in stat");
        strcpy(data, "INVALID");
    }
}