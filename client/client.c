#include "client.h"

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

int mynfs_open(char *path, int flags, int mode) {
  char com[64] = "mynfs_open ";
  char sflags[4], smode[4];
  int fd;
  strcat(com, path);
  
  sprintf(sflags, "%d", flags);
  sprintf(smode, "%d", mode);
  
  strcat(com, " ");
  strcat(com, sflags);
  strcat(com, " ");
  strcat(com, smode);

  if(write(sock, com, 1024) == -1) {
  	//mynfs_error = 1;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    //mynfs_error = ;
    return -1;
  }
  
  
  if(read(sock, &fd, sizeof(int)) == -1) {
  	//mynfs_error = 2;
  }
  
  printf("fd %d\n", fd);
  send_success();
  
  if(fd != -1) {
  	add_opened_file(fd, path, flags);
  }
  
  printf("mynfs_open issued\n");
  return fd;
}

int mynfs_close(int fd) {
  char sfd[4];
  char com[64] = "mynfs_close ";
  int response, result = 0;
  
  sprintf(sfd, "%d", fd);
  strcat(com, sfd);
  
  if(write(sock, com, 1024) == -1) {
  	//mynfs_error = 3;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    //mynfs_error = ;
    return -1;
  }
  
  
  if(read(sock, &response, sizeof(int)) == -1) {
  	//mynfs_error = 4;
  }
  
  printf("response = %d\n", response);
  
  if(response == 0) {
  	result = -1;
  	//mynfs_error = 5;
  }
  
  remove_opened_file(fd);
  printf("mynfs_close issued\n");
  return result;
}

ssize_t mynfs_read(int fd, void *buf, size_t size) {
  printf("mynfs_read issued\n");
  char com[64] = "mynfs_read ";
  char sfd[4], ssize[4];
  int n = 0;
  
  char *buf2;
  sprintf(sfd, "%d", fd);
  sprintf(ssize, "%d", (int)size);
  
  strcat(com, sfd);
  strcat(com, " ");
  strcat(com, ssize);
  printf("%s sent\n", com);
  write(sock, com, 1024);
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    //mynfs_error = ;
    return -1;
  }
  
  read(sock, &n, sizeof(int));
  printf("%d received\n", n);
  read(sock, buf, n);
  buf2 = (char*) buf;
  printf("buf: %s\n", buf2);

  return n;
}

ssize_t mynfs_write(int fd, const void *buf, size_t count) {
  char com[64] = "mynfs_write ";
  char sfd[4], sn[4];
  int res;
  
  
  sprintf(sfd, "%d", fd);
  strcat(com, sfd);
  strcat(com, " ");
  sprintf(sn, "%d", (int)count);
  strcat(com, sn);
  printf("%s\n", com);
  write(sock, com, 1024);
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    //mynfs_error = ;
    return -1;
  }
  
  printf("sent\n");

  write(sock, buf, count);
  read(sock, &res, sizeof(int));
  return res;
}

off_t mynfs_lseek(int fd, off_t offset, int whence) {
	printf("mynfs_lseek issued\n");
	int result;
	char sfd[4], soffset[4], swhence[4];
  char com[64] = "mynfs_lseek ";
  
  sprintf(sfd, "%d", fd);
  sprintf(soffset, "%d", (int)offset);
  sprintf(swhence, "%d", whence);
  
  strcat(com, sfd);
  strcat(com, " ");
  strcat(com, soffset);
  strcat(com, " ");
  strcat(com, swhence);
  
  if(write(sock, com, 1024) == -1) {
  	//mynfs_error = ;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    //mynfs_error = ;
    return -1;
  }
  
  
  if(read(sock, &result, sizeof(int)) == -1) {
  	send_failure();
  } else {
  	send_success();
  }
  
  printf("lseek result = %d\n", result);  
  return result;
}

int mynfs_unlink(char *path) {
	int response, result = -1;
  char com[64] = "mynfs_unlink ";
  strcat(com, path);
  
  if(write(sock, com, 1024) == -1) {
  	//mynfs_error = ;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    //mynfs_error = ;
    return -1;
  }
  
  
  if(read(sock, &response, sizeof(int)) == -1) {
  	//mynfs_error = ;
  }
  
  if(response == 1) result = 0;
  printf("response = %d\n", response);
  return result;
}

int mynfs_fstat(int fd, struct stat *buf) {
  char sfd[4], com[64] = "mynfs_fstat ";
  
  sprintf(sfd, "%d", fd);
  strcat(com, sfd);
  write(sock, com, 1024);
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    //mynfs_error = ;
    return -1;
  }
  
  printf("mynfs_fstat issued\n");
  
  int res;
  read(sock, &res, sizeof(int));
  if(!res) {
    //mynfs_error = ;
    return -1;
  }
  
  read(sock, &buf->st_dev, sizeof(dev_t));
  read(sock, &buf->st_ino, sizeof(ino_t));
  read(sock, &buf->st_mode, sizeof(mode_t));
  read(sock, &buf->st_nlink, sizeof(nlink_t));
  read(sock, &buf->st_uid, sizeof(uid_t));
  read(sock, &buf->st_gid, sizeof(gid_t));
  read(sock, &buf->st_rdev, sizeof(dev_t));
  read(sock, &buf->st_size, sizeof(off_t));
  read(sock, &buf->st_blksize, sizeof(blksize_t));
  read(sock, &buf->st_blocks, sizeof(blkcnt_t));
  read(sock, &buf->st_atime, sizeof(time_t));
  read(sock, &buf->st_mtime, sizeof(time_t));
  read(sock, &buf->st_ctime, sizeof(time_t));
  
  return 1;
}

int mynfs_opendir(char *path) {
	int dd;
  char com[64] = "mynfs_opendir ";
  strcat(com, path);
  
  if(write(sock, com, 1024) == -1) {
  	//mynfs_error
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    //mynfs_error = ;
    return -1;
  }
  
  if(read(sock, &dd, sizeof(int)) == -1) {
  	//mynfs_error = 2;
  }
  
  printf("dd %d\n", dd);
  
  if(dd != -1) {
  	send_success();
  	add_opened_dir(dd, path);
  } else {
    send_failure();
  }
  
  return dd;
}

int mynfs_closedir(int dd) {
	int response, result = 0;
  char sdd[4], com[64] = "mynfs_closedir ";
  
  sprintf(sdd, "%d", dd);
  strcat(com, sdd);
  
  if(write(sock, com, 1024) == -1) {
  	//mynfs_error
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    //mynfs_error = ;
    return -1;
  }
  
  if(read(sock, &response, sizeof(int)) == -1) {
  	//mynfs_error = 4;
  }
  
  printf("response = %d\n", response);
  
  if(response == 0) {
  	result = -1;
  	//mynfs_error = 5;
  }
  
  remove_opened_dir(dd);
  printf("mynfs_closedir issued\n");
  return result;
}

int mynfs_readdir(int dd) {
  char com[64] = "mynfs_readdir ";
  char buf[1024], val[64], sdd[4];
  
  sprintf(sdd, "%d", dd);
  strcat(com, sdd);
  write(sock, com, 1024);
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    //mynfs_error = ;
    return -1;
  }
  
  read(sock, buf, 1024);  
  send_success();
  
  if(strcmp(buf, "")) {
    return -1;
  }
  
  printf("readdir result:\n");
  strcpy(val, strtok(buf, "/"));
  
  while(1) {
    printf("->%s\n", val);
    char *tmp = strtok(NULL, "/");
    
    if(tmp == NULL) {
      break;
    }
    
    strcpy(val, tmp);
  }
  
  printf("mynfs_readdir issued\n");
  return 1;
}

void client_exec() {
  while(1) {
    int n = 64;
    char *str, getstr[n], *com, *arg;
    str = fgets(getstr, n, stdin);
    // if(str == NULL)

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
      char *path;
      int flags, mode;
      
      path = strtok(arg, " ");
      flags = atoi(strtok(NULL, " "));
      mode = atoi(strtok(NULL, " "));
      
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
      
      dd = atoi(strtok(arg, " "));
      
      res = mynfs_readdir(dd);
    } if(!strcmp(com, "mynfs_closedir")) {
      int dd;
      
      dd = atoi(strtok(arg, " "));
      
      res = mynfs_closedir(dd);
    } if(!strcmp(com, "mynfs_fstat")) {
      struct stat buf;
      int fd;
      
      fd = atoi(strtok(arg, " "));
      
      res = mynfs_fstat(fd, &buf);
      if(res == 1) {
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
      
    list_all();
  }
  close(sock);
}
