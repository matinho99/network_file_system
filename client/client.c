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
