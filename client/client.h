#ifndef CLIENT_H
#define CLIENT_H

#include "mynfs_utility.h"
/*
int mynfs_open(char *host, char *path, int flags, int mode);

int mynfs_close();

int mynfs_read(int fd, void *buf, int size);

int mynfs_write(int fd, void *buf, int size);

int mynfs_lseek(int fd, int offset, int whence);

int mynfs_unlink(int fd);

int mynfs_opendir(char *host, char *path);

int mynfs_closedir(int dirfd);

int mynfs_readdir(int dirfd);

int mynfs_fstat(int fd);
*/
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

#endif
