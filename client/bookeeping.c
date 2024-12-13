#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

void book(char *str)
{
    const char *file_path = "../bookeeping.txt";
    FILE *file = fopen(file_path, "a");
    if (file == NULL)
    {
        fprintf(stderr, "Unable to open file for writing: %s\n", file_path);
        return;
    }
    fprintf(file, "%s\n", str);
    fclose(file);
}