#include "basic_header.h"
#include "storageservers.h"
void modifyFilePath(char *filePath)
{
    char *fileName = strrchr(filePath, '/');
    char extend[200];
    int flag = 0;

    if (fileName != NULL)
    {
        fileName++;
    }
    else
    {
        fileName = filePath;
    }

    char *extension = strrchr(fileName, '.');
    if (extension != NULL)
    {
        strcpy(extend, extension);
        *extension = '\0';
    }
    else
    {
        flag = 1;
    }

    // time_t rawtime;
    // struct tm *timeinfo;
    // char timestamp[30];

    // time(&rawtime);
    // timeinfo = localtime(&rawtime);
    // strftime(timestamp, sizeof(timestamp), "_%Y%m%d%H%M%S", timeinfo);

    // strcat(fileName, timestamp);
    if (flag != 1)
        strcat(fileName, extend);
}
void copyfile(char *path, int socket, char *destination, int flag)
{
    ptrtocopysender copysender = malloc(sizeof(struct copysender));
    copysender->number = 1;
    copysender->type = 1;
    strcpy(copysender->data, destination);
    strcat(copysender->data, "/");
    strcat(copysender->data, basename((char *)path));
    if (flag >= 1)
    {
        // printf("here\n");
        modifyFilePath(copysender->data);
    }

    // tcflush(socket,TCIOFLUSH);
    ssize_t s1 = send(socket, copysender, sizeof(struct copysender), 0);
    // fflush(socket);
    printf("%d %d %s %ld\n", copysender->number, copysender->type, copysender->data, s1);
    if (s1 < 0)
    {
        perror("Error in sending data");
    }
    int res = -1;
    recv(socket, &res, 4, 0);
    printf("%d\n", res);

    // printf("hiiii %s\n", response);
    if (flag == 2)
    {
        createfile(copysender->data, 1);
        int fd = open(path, O_RDONLY);
        if (fd == -1)
        {
            perror("Error opening file");
        }
        int fd1 = open(copysender->data, O_WRONLY | O_TRUNC);
        if (fd1 == -1)
        {
            perror("Error opening file");
        }
        char data[MAX_BUFFER_AT_ONE_TIME];
        while (1)
        {
            int a = readdata(data, fd);
            if (a != 0 && a != -1)
                writedata(data, fd1);
            else
                break;
        }
        close(fd);
        close(fd1);
        return;
    }
    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
    }
    int a = -1;
    while (1)
    {
        // printf("hj\n");
        copysender->number = 100;
        a = readdata(copysender->data, fd);
        // printf("%d\n", a);
        if (a != 0 && a != -1)
        {
            ssize_t s2 = send(socket, copysender, sizeof(struct copysender), 0);
            printf("%d %d %s\n", copysender->number, copysender->number, copysender->data);

            if (s2 < 0)
            {
                perror("Error in sending data");
            }
            res = -1;
            recv(socket, &res, 4, 0);
            // printf("%s\n", response);
            printf("%d\n", res);
            // printf("%s\n", "OK");
        }
        else
        {
            // printf("jjjj\n");
            copysender->number = -1;
            copysender->data[0] = '\0';

            ssize_t s2 = send(socket, copysender, sizeof(struct copysender), 0);
            if (s2 < 0)
            {
                perror("Error in sending data");
            }
            res = -1;
            int r9 = recv(socket, &res, 4, 0);
            printf("%d\n", res);

            if (r9 < 0)
            {
                perror("Error recieving data");
            }
            // printf("%s\n", response);

            break;
        }
    }
    free(copysender);
    close(fd);
}
void sendFilesRecursively(int socket, char *basePath, char *destination, int flag)
{
    DIR *dir = opendir(basePath);
    if (!dir)
    {
        perror("opendir");
        return;
    }

    struct dirent *entry1;
    char path[MAX_PATH_LENGTH + 1];
    char dest[MAX_PATH_LENGTH + 1];
    while ((entry1 = readdir(dir)) != NULL)
    {
        memset(path, '\0', MAX_PATH_LENGTH);
        memset(dest, '\0', MAX_PATH_LENGTH);
        if (strcmp(entry1->d_name, ".") == 0 || strcmp(entry1->d_name, "..") == 0)
        {
            continue;
        }
        int flag1 = 0;
        snprintf(path, sizeof(path), "%s/%s", basePath, entry1->d_name);
        for (int i = 0; i < entry.numberofpaths; i++)
        {
            if (strcmp(entry.pathsaccessible[i], path) == 0)
            {
                flag1 = 1;
                break;
            }
        }
        if (flag1 == 0)
        {
            continue;
        } 
        // ask for assumption
        // if all things should be copied without checking if it is in accessible path or not then comment this else uncomment

        struct stat st;
        printf("heyyyyyyyyyyyyy%s\n",path);
        if (stat(path, &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
            {
                snprintf(dest, sizeof(dest), "%s/%s", destination, entry1->d_name);
                if (flag >= 1)
                {
                    modifyFilePath(dest);
                }
                ptrtocopysender copy = malloc(sizeof(struct copysender));
                copy->type = 0;

                copy->number = 1;
                if (flag == 2)
                {
                    createfile(dest, 0);
                }

                strcpy(copy->data, dest);
                ssize_t s3 = send(socket, copy, sizeof(struct copysender), 0);
                printf("%d %d %s\n", copy->type, copy->number, copy->data);

                if (s3 < 0)
                {
                    perror("Error in sending data");
                }
                int res = -1;
                recv(socket, &res, 4, 0);
                printf("%d\n", res);
                free(copy);
                sendFilesRecursively(socket, path, dest, flag);
            }
            else if (S_ISREG(st.st_mode))
            {
                copyfile(path, socket, destination, flag);
            }
        }
    }

    closedir(dir);
}

void copy1(char *path, int socket, char *destination, int flag)
{
    struct stat path_stat;
    ptrtocopysender copysender = malloc(sizeof(struct copysender));
    printf("%s\n", path);
    stat(path, &path_stat);
    if (S_ISREG(path_stat.st_mode))
    {
        // print3f("heloooooooooooo\n");
        copyfile(path, socket, destination, flag);
    }
    else if (S_ISDIR(path_stat.st_mode))
    {
        copysender->number = 1;
        copysender->type = 0;
        strcpy(copysender->data, destination);
        strcat(copysender->data, "/");
        strcat(copysender->data, basename((char *)path));
        printf("%s\n", copysender->data);
        if (flag >= 1)
        {
            modifyFilePath(copysender->data);
        }
        if (flag == 2)
        {
            createfile(copysender->data, 0);
        }
        printf("%d %d %s\n", copysender->type, copysender->number, copysender->data);

        ssize_t s1 = send(socket, copysender, sizeof(struct copysender), 0);
        printf("%d %d %s %ld\n", copysender->type, copysender->number, copysender->data, s1);

        // printf("%s\n", copysender->data);

        if (s1 < 0)
        {
            perror("Error in sending data");
        }
        int res = -1;
        recv(socket, &res, 4, 0);
        printf("%d\n", res);
        sendFilesRecursively(socket, path, copysender->data, flag);
    }
    copysender->number = -100;
    copysender->data[0] = '\0';
    ssize_t s2 = send(socket, copysender, sizeof(struct copysender), 0);
    if (s2 < 0)
    {
        perror("Error in sending data");
    }
    int res = -1;
    recv(socket, &res, 4, 0);
    printf("%d\n", res);
    free(copysender);
    // printf("here\n");
}