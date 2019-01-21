#ifndef MYNFS_UTILITY_H
#define MYNFS_UTILITY_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mynfs_error.h"

#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR 02
#define O_CREAT 0100
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2


int sock;

struct mynfs_opened_file {
  int file_descriptor;
  char filepath[50];
  int flags;
};

struct mynfs_opened_file_array {
  struct mynfs_opened_file opened_files[100];
  int num_opened_files;
} opened_files_arr;

struct mynfs_opened_dir {
  int dir_descriptor;
  char dirpath[50];
} opened_dirs[100];

struct mynfs_opened_dir_array {
  struct mynfs_opened_dir opened_dirs[100];
  int num_opened_dirs;
} opened_dirs_arr;


void init_client_socket(char *host, char *port);

void add_opened_file(int fd, char *path, int flags);

void remove_opened_file(int fd);

void add_opened_dir(int dd, char *path);

void remove_opened_dir(int dd);

void list_all();

void send_success();

void send_failure();

#endif
