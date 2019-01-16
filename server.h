#ifndef SERVER_H
#define SERVER_H

#include "mynfs_utility.h"
#include "mynfs_error.h"

int server_sock;

int mynfs_open(struct client_info ci, char *path, int flags, int mode);

int mynfs_close(int fd);

int mynfs_read(struct client_info ci, int fd, void *buf, int size);

int mynfs_write(struct client_info ci, int fd, void *buf, int size);

int mynfs_lseek(int fd, int offset, int whence);

int mynfs_unlink(char *path);

int mynfs_fstat(int fd);

int mynfs_opendir(struct client_info ci, char *path);

int mynfs_closedir(int dd);

char *mynfs_readdir(int dd);

void exec_operation(char *message, struct client_info ci);

void server_exec();

#endif
