#include "mynfs_error.h"

/*
 * function: print_error
 *
 * prints specific error depending on the value of mynfs_error
 */
void print_error() {
  printf("ERROR: ");
  
  switch(mynfs_error) {
    case 1:
      printf("invalid command\n");
      break;
    case 2:
      printf("mynfs_open write com to socket failed\n");
      break;
    case 3:
      printf("mynfs_open read from socket failed\n");
      break;
    case 4:
      printf("mynfs_close write com to socket failed\n");
      break;
    case 5:
      printf("mynfs_close read from socket failed\n");
      break;
    case 6:
      printf("mynfs_read write com to socket failed\n");
      break;
    case 7:
      printf("mynfs_read read n from socket failed\n");
      break;
    case 8:
      printf("mynfs_read read buf from socket failed\n");
      break;
    case 9:
      printf("mynfs_write write com to socket failed\n");
      break;
    case 10:
      printf("mynfs_write write buf to socket failed\n");
      break;
    case 11:
      printf("mynfs_write read from socket failed\n");
      break;
    case 12:
      printf("mynfs_lseek write com to socket failed\n");
      break;
    case 13:
      printf("mynfs_lseek read from socket failed\n");
      break;
    case 14:
      printf("mynfs_unlink write com to socket failed\n");
      break;
    case 15:
      printf("mynfs_unlink read from socket failed\n");
      break;
    case 16:
      printf("mynfs_fstat write com to socket failed\n");
      break;
    case 17:
      printf("mynfs_fstat read from socket failed\n");
      break;
    case 18:
      printf("mynfs_opendir write com to socket failed\n");
      break;
    case 19:
      printf("mynfs_opendir read from socket failed\n");
      break;
    case 20:
      printf("mynfs_closedir write com to socket failed\n");
      break;
    case 21:
      printf("mynfs_closedir read from socket failed\n");
      break;
    case 22:
      printf("mynfs_readdir write com to socket failed\n");
      break;
    case 23:
      printf("mynfs_readdir read from socket failed\n");
      break;
    default:
      break;
  }
}
