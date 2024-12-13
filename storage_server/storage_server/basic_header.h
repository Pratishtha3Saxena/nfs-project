#ifndef _BHH_
#define _BHH_

#include <stdio.h>
#include <stdlib.h>
#include"termios.h"
#include <math.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pthread.h> 
#include <semaphore.h>
#include <fcntl.h>
#include"libgen.h"
#include <sys/wait.h>
#include"storageservers.h"
void *namingserver(void *arg);
void copy1(char *path, int socket, char *destination,int flag);
void additionalinfo(char *path, char *data);
int readdata(char *data,int fd);
int createfile(char *path,int number);
int delete1(char *path, int number);
void storageserverinitiailise(int port, char *ip, int portforclientcommunication, struct entries *entry, char *path);
int writedata(char *data,int fd);
void paste(char *path, int socket);

    


#endif


