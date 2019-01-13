#ifndef SERVER_H
#define SERVER_H

#include "mynfs_utility.h"
#include "mynfs_error.h"

int server_sock;

int mynfs_open(char *host, char *path, int flags, int mode);

int mynfs_close(int fd);

int mynfs_read(int fd, void *buf, int size);

int mynfs_write(int fd, void *buf, int size);

int mynfs_lseek(int fd, int offset, int whence);

int mynfs_unlink(int fd);

int mynfs_fstat(int fd);

int mynfs_opendir(char *host, char *path);

int mynfs_closedir(int dirfd);

int mynfs_readdir(int dirfd);

void exec_operation(char *message, struct client_info ci);

void server_exec();

#endif
