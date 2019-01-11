#include "client.h"

void help() {
  printf("Available commands:\n");
  printf("mynfs_open <host> <path> <flags> <mode>\n");
  printf("mynfs_read\n");
  printf("mynfs_write <path to the local file>\n");
  printf("mynfs_lseek <offset> <whence>\n");
  printf("mynfs_close\n");
  printf("mynfs_unlink <host> <path>\n");
  printf("mynfs_opendir <host> <path>\n");
  printf("mynfs_readdir\n");
  printf("mynfs_closedir\n");
  printf("mynfs_fstat\n");
}

int mynfs_open(char *arg) {
  printf("mynfs_open");
  return 1;
}

int mynfs_close() {
  printf("mynfs_close");
  return 1;
}

int mynfs_read() {
  printf("mynfs_read");
  return 1;
}

int mynfs_write(char *arg) {
  printf("mynfs_write");
  return 1;
}

int mynfs_lseek(char *arg) {
  printf("mynfs_lseek");
  return 1;
}

int mynfs_unlink(char *arg) {
  printf("mynfs_unlink");
  return 1;
}

int mynfs_opendir(char *arg) {
  printf("mynfs_opendir");
  return 1;
}

int mynfs_closedir() {
  printf("mynfs_closed");
  return 1;
}

int mynfs_readdir() {
  printf("mynfs_readdir");
  return 1;
}

int mynfs_fstat() {
  printf("mynfs_fstat");
  return 1;
}
