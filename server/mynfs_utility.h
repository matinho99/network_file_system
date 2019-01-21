#ifndef MYNFS_UTILITY_H
#define MYNFS_UTILITY_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include "mynfs_error.h"

#define PORT 8080
#define MAX_CLIENTS 10
#define TIMEOUT_SEC 60
#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR 02
#define O_CREAT 0100
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/* contains information about access a client has to specified place in the file system */
struct mynfs_access {
  char client_ip[20];
  char path[50];
  int flags;
};

/* contains array of accesses */
struct mynfs_access_array {
  struct mynfs_access *client_accesses;
  int size;
} client_accesses_arr;

/* contains information about clients' opened files */
struct mynfs_opened_file {
  int file_descriptor;
  char filepath[50];
  char client_ip[20];
  int flags;
};

/* contains array of opened files */
struct mynfs_opened_file_array {
  struct mynfs_opened_file opened_files[100];
  int num_opened_files;
} opened_files_arr;

/* contains information about clients' opened directories */
struct mynfs_opened_dir {
  int dir_descriptor;
  char dirpath[50];
  char client_ip[20];
} opened_dirs[100];

/* contains array of opened directories */
struct mynfs_opened_dir_array {
  struct mynfs_opened_dir opened_dirs[100];
  int num_opened_dirs;
} opened_dirs_arr;

/* contains information about client - his socket and IP address */
struct client_info {
  int sock;
  char ip[20];
} client_sockets[MAX_CLIENTS];

void init_server_socket(int *server_sock);

void load_client_accesses();

void add_opened_file(struct client_info ci, int fd, char *path, int flags);

void remove_opened_file(struct client_info ci, int fd);

void add_opened_dir(struct client_info ci, int dd, char *path);

void remove_opened_dir(struct client_info ci, int dd);

int has_access_to_dir(struct client_info ci, char *dp);

int has_access_to_file(struct client_info ci, char *fp, int mode);

int has_opened_file(struct client_info ci, int fd);

int has_opened_file_by_path(struct client_info ci, char *path);

int has_opened_dir(struct client_info ci, int dd);

int has_opened_dir_by_path(struct client_info ci, char *path);

int has_read_access(struct client_info ci, int fd);

int has_write_access(struct client_info ci, int fd);

void send_success(struct client_info ci);

void send_failure(struct client_info ci);

#endif
