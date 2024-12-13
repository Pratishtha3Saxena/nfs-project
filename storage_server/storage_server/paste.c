#include "basic_header.h"
#include "macros.h"
#include "storageservers.h"
void paste(char *path, int socket)
{
    ptrtocopysender copy = malloc(sizeof(struct copysender));
    while (1)
    {
        memset(copy->data, '\0', MAX_BUFFER_AT_ONE_TIME);
        // printf("htt\n");
        ssize_t r1 = recv(socket, copy, sizeof(struct copysender), 0);
        // printf("adios %s %d %d\n", copy->data, copy->number, copy->type);
        if (r1 < 0)
        {
            perror("Error in receiving data");
        }
        int res = -1;
        int s3 = send(socket, &res, 4, 0);
        if (s3 < 0)
        {
            perror("Error in sending data");
        }
        if (copy->number == 1)
        {
            createfile(copy->data, copy->type);

            if (copy->type == 1)
            {
                path = (char *)malloc(MAX_BUFFER_AT_ONE_TIME);
                strcpy(path, copy->data);
                int fd = open(copy->data, O_WRONLY);
                if (fd == -1)
                {
                    perror("Error in opening file");
                }
                while (1)
                {
                    ssize_t r2 = recv(socket, copy, sizeof(struct copysender), 0);
                    // printf("holaa %s %ld %d %d\n", copy->data,strlen(copy->data), copy->number, copy->type);

                    if (r2 < 0)
                    {
                        perror("Error in receiving data");
                    }
                    s3 = send(socket, &res, 4, 0);
                    if (s3 < 0)
                    {
                        perror("Error in sending data");
                    }

                    if (copy->number == -1)
                    {

                        break;
                    }
                    else
                    {
                        writedata(copy->data, fd);
                    }
                }
                close(fd);
            }
        }
        else if (copy->number == -100)
        {
            break;
        }
    }
    free(copy);
}