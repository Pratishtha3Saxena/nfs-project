#include "basic_header.h"

int flag = 1;
void deleteFilesInFolder(char *folderPath) {
    struct dirent *entry;
    DIR *dir = opendir(folderPath);

    if (dir == NULL) {
        flag = 0;
        printf("Unable to open directory %s\n", folderPath);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char path[1000];
            snprintf(path, sizeof(path), "%s/%s", folderPath, entry->d_name);

            struct stat statbuf;
            if (stat(path, &statbuf) == 0) {
                if (S_ISDIR(statbuf.st_mode)) {
                    deleteFilesInFolder(path); 
                    if (rmdir(path) != 0) {
                        flag = 1;
                        printf("Unable to delete directory: %s\n", path);
                    }
                } else {
                    if (unlink(path) != 0) {
                        flag = 1;
                        printf("Unable to delete file: %s\n", path);
                    }
                }
            }
        }
    }

    
    closedir(dir);
    rmdir(folderPath);
}
int delete1(char *path, int number)
{
    // printf("HERE\n");
    // printf("%s\n", path);
    if (number == 0)
    {
        deleteFilesInFolder(path);
        return flag;
    }
    else
    {
        if (unlink(path) == 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}