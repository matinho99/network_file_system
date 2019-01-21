#include "client.h"

/*
 * function: help
 *
 * prints commands available for use with client_exec()
 */
void help() {
  printf("Available commands:\n");
  printf("mynfs_open <path> <flags> <mode>\n");
  printf("mynfs_read <fd> <path to local file> <n>\n");
  printf("mynfs_write <path to the local file> <fd> <n>\n");
  printf("mynfs_lseek <fd> <offset> <whence>\n");
  printf("mynfs_close <fd>\n");
  printf("mynfs_unlink <path>\n");
  printf("mynfs_opendir <path>\n");
  printf("mynfs_readdir <dd>\n");
  printf("mynfs_closedir <dd>\n");
  printf("mynfs_fstat <fd>\n");
  printf("list_all\n");
}

/*
 * function: client_exec
 *
 * handles the input from the console
 */
void client_exec() {
  while(1) {
    int n = 64;
    char *str, getstr[n], *com, *arg;
    printf(">");
    str = fgets(getstr, n, stdin);
    
    str = strtok(str, "\n");
    com = strtok(str, " ");
    if(com == NULL)
      com = str;
    else
      arg = strtok(NULL, "\0");

    int res;

    if(!strcmp(com, "help")) {
      help();
    } else if(!strcmp(com, "mynfs_open")) {
      char *path, *sflags, *smode;
      int flags, mode;
      
      path = strtok(arg, " ");
      sflags = strtok(NULL, " ");
      smode = strtok(NULL, " ");
      if(smode != NULL) {
        mode = atoi(smode);
      } else {
        mode = 0;
      }
      if(!strcmp(sflags, "O_RDONLY")) flags = O_RDONLY;
      else if(!strcmp(sflags, "O_WRONLY")) flags = O_WRONLY;
      else if(!strcmp(sflags, "O_RDWR")) flags = O_RDWR;
      if(!strcmp(sflags, "O_RDONLY|O_CREAT")) flags = (O_RDONLY|O_CREAT);
      else if(!strcmp(sflags, "O_WRONLY|O_CREAT")) flags = (O_WRONLY|O_CREAT);
      else if(!strcmp(sflags, "O_RDWR|O_CREAT")) flags = (O_RDWR|O_CREAT);
      
      res = mynfs_open(path, flags, mode);
    } else if(!strcmp(com, "mynfs_read")) {
      char buf[1024], *path;
      int local_fd, fd, size;
      
      fd = atoi(strtok(arg, " "));
      path = strtok(NULL, " ");
      size = atoi(strtok(NULL, " "));
      
      local_fd = open(path, (O_RDWR|O_CREAT), 00700);
      printf("%d\n", local_fd);
      
      if(local_fd == -1) {
        mynfs_error = 6;
      } else {
        res = mynfs_read(fd, buf, (size_t)size);
      
        if(write(local_fd, buf, res) == -1) {
          mynfs_error = 8;
        }
      }
    } if(!strcmp(com, "mynfs_write")) {
      char buf[1024], *path;
      int local_fd, rs, fd, size;
      
      path = strtok(arg, " ");
      fd = atoi(strtok(NULL, " "));
      size = atoi(strtok(NULL, " "));
      
      local_fd = open(path, O_RDONLY);
      
      if(local_fd == -1) {
        mynfs_error = 7;
      } else {
        
        rs = read(local_fd, buf, size);
        printf("%s\n", buf);
        
        res = mynfs_write(fd, buf, rs);
        printf("%d\n", res);
      }
    } if(!strcmp(com, "mynfs_lseek")) {
      int fd, offset, whence;
      
      fd = atoi(strtok(arg, " "));
      offset = atoi(strtok(NULL, " "));
      whence = atoi(strtok(NULL, " "));
      
      res = mynfs_lseek(fd, offset, whence);
      printf("%d\n", res);
    } if(!strcmp(com, "mynfs_close")) {
      int fd;
      
      fd = atoi(strtok(arg, " "));
      
      res = mynfs_close(fd);
    } if(!strcmp(com, "mynfs_unlink")) {
      res = mynfs_unlink(arg);
    } if(!strcmp(com, "mynfs_opendir")) {
      res = mynfs_opendir(arg);
    } if(!strcmp(com, "mynfs_readdir")) {
      int dd;
      char val[64], buf[1024];
      
      dd = atoi(strtok(arg, " "));
      
      res = mynfs_readdir(dd, buf);
      
      strcpy(val, strtok(buf, "/"));
      
      while(1) {
        printf("->%s\n", val);
        char *tmp = strtok(NULL, "/");
    
        if(tmp == NULL) {
          break;
        }
        
        strcpy(val, tmp);
      }
    } if(!strcmp(com, "mynfs_closedir")) {
      int dd;
      
      dd = atoi(strtok(arg, " "));
      
      res = mynfs_closedir(dd);
    } if(!strcmp(com, "mynfs_fstat")) {
      struct stat buf;
      int fd;
      
      fd = atoi(strtok(arg, " "));
      
      res = mynfs_fstat(fd, &buf);
      if(res == 0) {
        printf("\nst_dev: %d\n", (int)buf.st_dev);
        printf("st_ino: %d\n", (int)buf.st_ino);
        printf("st_mode: %d\n", (int)buf.st_mode);
        printf("st_nlink: %d\n", (int)buf.st_nlink);
        printf("st_uid: %d\n", (int)buf.st_uid);
        printf("st_gid: %d\n", (int)buf.st_gid);
        printf("st_rdev: %d\n", (int)buf.st_rdev);
        printf("st_size: %d\n", (int)buf.st_size);
        printf("st_blksize: %d\n", (int)buf.st_blksize);
        printf("st_blocks: %d\n", (int)buf.st_blocks);
        printf("st_atime: %d\n", (int)buf.st_atime);
        printf("st_mtime: %d\n", (int)buf.st_mtime);
        printf("st_ctime: %d\n\n", (int)buf.st_ctime);
      }
    } if(!strcmp(com, "list_all")) {
      list_all();
    } if(!strcmp(com, "close")) {
      break;
    }
    
    if(mynfs_error != 0) {
      print_error();
      mynfs_error = 0;
    }
  }
  close(sock);
}

int main(int argc, char *argv[])
{
  if(argc != 3) {
    printf("Invalid number of arguments\n");
    exit(-1);
  }

  init_client_socket(argv[1], argv[2]);
  client_exec();
  exit(0);
}
