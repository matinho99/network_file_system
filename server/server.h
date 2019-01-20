#ifndef SERVER_H
#define SERVER_H

#include "mynfs_utility.h"
#include "mynfs_error.h"

int server_sock; /* server socket */

int mynfs_open(struct client_info ci, char *path, int flags, int mode);

int mynfs_close(int fd);

int mynfs_read(int fd, void *buf, int size);

int mynfs_write(int fd, void *buf, int size);

int mynfs_lseek(int fd, int offset, int whence);

int mynfs_unlink(char *path);

int mynfs_fstat(struct client_info ci, int fd);

int mynfs_opendir(struct client_info ci, char *path);

int mynfs_closedir(int dd);

int mynfs_readdir(struct client_info ci, int dd);

void exec_operation(char *message, struct client_info ci);

void server_exec();

void server_close();

#endif
