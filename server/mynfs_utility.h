#ifndef MYNFS_UTILITY_H
#define MYNFS_UTILITY_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define O_RDONLY 100
#define O_WRONLY 101
#define O_RDWR 102
#define O_CREAT 103

struct mynfs_access {
  char client_ip[20];
  char path[50];
  int mode;
};

struct mynfs_access_array {
  struct mynfs_access *client_accesses;
  int size;
} client_accesses_arr;

struct mynfs_opened_file {
  int file_descriptor;
  char client_ip[20];
  int access_level;
};

struct mynfs_opened_file_array {
  struct mynfs_opened_file opened_files[100];
  int num_opened_files;
} opened_files_arr;

struct mynfs_opened_dir {
  int dir_descriptor;
  char client_ip[20];
} opened_dirs[100];

struct mynfs_opened_dir_array {
  struct mynfs_opened_dir opened_dirs[100];
  int num_opened_dirs;
} opened_dirs_arr;

struct client_info {
  int sock;
  char ip[20];
} client_sockets[MAX_CLIENTS];

void init_server_socket(int *server_sock);

void load_client_accesses();

int has_access_to_file(struct client_info ci, char *fp, int mode);

int has_access_to_dir(struct client_info ci, char *dp);

int has_opened_file(struct client_info ci, int fd);

int has_opened_dir(struct client_info ci, int dd);

void send_success(struct client_info ci);

void send_failure(struct client_info ci);

#endif
