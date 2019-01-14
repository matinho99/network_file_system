#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mynfs_utility.h"

int sock;

void help();

int mynfs_open(char *arg);

int mynfs_close();

int mynfs_read();

int mynfs_write(char *arg);

int mynfs_lseek(char *arg);

int mynfs_unlink(char *arg);

int mynfs_opendir(char *arg);

int mynfs_closedir();

int mynfs_readdir();

int mynfs_fstat();

void init_client_socket(char *host, char *port);

void client_exec();

#endif
