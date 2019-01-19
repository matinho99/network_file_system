#ifndef CLIENT_H
#define CLIENT_H

#include "mynfs_utility.h"

int sock;

void help();

int mynfs_open(char *arg);

int mynfs_close(char *arg);

int mynfs_read(char *arg);

int mynfs_write(char *arg);

int mynfs_lseek(char *arg);

int mynfs_unlink(char *arg);

int mynfs_fstat(char *arg);

int mynfs_opendir(char *arg);

int mynfs_closedir(char *arg);

int mynfs_readdir(char *arg);

void init_client_socket(char *host, char *port);

void client_exec();

#endif
