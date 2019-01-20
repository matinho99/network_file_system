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
      printf("file open failed\n");
      break;
    case 2:
      printf("desriptor write to socket failed\n");
      break;
    case 3:
      printf("response read from socket failed\n");
      break;
    case 4:
      printf("file close failed\n");
      break;
    case 5:
      printf("read from file failed\n");
      break;
    case 6:
      printf("write to file failed\n");
      break;
    case 7:
      printf("lseek failed\n");
      break;
    case 8:
      printf("unlink file failed\n");
      break;
    case 9:
      printf("fstat failed\n");
      break;
    case 10:
      printf("opendir failed\n");
      break;
    case 11:
      printf("closedir failed\n");
      break;
    case 12:
      printf("readdir failed\n");
      break;
    case 13:
      printf("write mynfs_close result to socket failed\n");
      break;
    case 14:
      printf("write mynfs_read result to socket failed\n");
      break;
    case 15:
      printf("write data from file to socket failed\n");
      break;
    case 16:
      printf("read data from socket failed\n");
      break;
    case 17:
      printf("write mynfs_write result to socket failed\n");
      break;
    case 18:
      printf("write mynfs_lseek result to socket failed\n");
      break;
    case 19:
      printf("write mynfs_unlink result to socket failed\n");
      break;
    case 20:
      printf("write mynfs_closedir result to socket failed\n");
      break;
    default:
      break;
  }
}
