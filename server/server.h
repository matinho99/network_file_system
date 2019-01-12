#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "mynfs_utility.h"

#define PORT 8080
#define MAX_CLIENTS 10

int server_sock, client_sockets[MAX_CLIENTS];

int mynfs_open(char *host, char *path, int flags, int mode);

int mynfs_close(int fd);

int mynfs_read(int fd, void *buf, int size);

int mynfs_write(int fd, void *buf, int size);

int mynfs_lseek(int fd, int offset, int whence);

int mynfs_unlink(int fd);

int mynfs_opendir(char *host, char *path);

int mynfs_closedir(int dirfd);

int mynfs_readdir(int dirfd);

int mynfs_fstat(int fd);

void init_server_socket();

void server_exec();

void decode_message();

#endif
