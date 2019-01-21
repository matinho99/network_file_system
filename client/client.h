#ifndef CLIENT_H
#define CLIENT_H

#include "mynfs_utility.h"

int mynfs_open(char *path, int flags, int mode);

int mynfs_close(int fd);

ssize_t mynfs_read(int fd, void *buf, size_t size);

ssize_t mynfs_write(int fd, const void *buf, size_t count);

off_t mynfs_lseek(int fd, off_t offset, int whence);

int mynfs_unlink(char *path);

int mynfs_fstat(int fd, struct stat *buf);

int mynfs_opendir(char *path);

int mynfs_closedir(int dd);

int mynfs_readdir(int dd, char *buf);

#endif
