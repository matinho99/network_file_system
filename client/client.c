#include "client.h"

/*
 * function: mynfs_open
 *
 * sends a request to open the server file at path with flags
 * and optionally mode (if O_CREAT has been used in flags)
 *
 * path - path to the file on the server
 * flags - flags specifying how to open the file (O_RDONLY, O_WRONLY, O_RDWR, O_CREAT)
 * mode - required if the O_CREAT flag has been used
 *
 * returns: server file descriptor if successful; -1 if failed
 */
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
  	mynfs_error = 2;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    mynfs_error = 1;
    return -1;
  }
  
  
  if(read(sock, &fd, sizeof(int)) == -1) {
  	mynfs_error = 3;
  }
  
  printf("fd %d\n", fd);
  send_success();
  
  if(fd != -1) {
  	add_opened_file(fd, path, flags);
  }
  
  printf("mynfs_open issued\n");
  return fd;
}

/*
 * function: mynfs_close
 *
 * sends a request to close the server file specified by fd
 *
 * fd - file descriptor
 *
 * returns: 0 if successful; -1 if failed
 */
int mynfs_close(int fd) {
  char sfd[4];
  char com[64] = "mynfs_close ";
  int response, result = 0;
  
  sprintf(sfd, "%d", fd);
  strcat(com, sfd);
  
  if(write(sock, com, 1024) == -1) {
  	mynfs_error = 4;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    mynfs_error = 1;
    return -1;
  }
  
  
  if(read(sock, &response, sizeof(int)) == -1) {
  	mynfs_error = 5;
  }
  
  printf("response = %d\n", response);
  
  if(response == 0) {
  	result = -1;
  }
  
  remove_opened_file(fd);
  printf("mynfs_close issued\n");
  return result;
}

/*
 * function: mynfs_read
 *
 * sends a request to read size number of bytes from the server file
 * specified by fd to buf
 *
 * fd - directory descriptor
 * buf - buffer to read to
 * size - number of bytes to read
 *
 * returns: number of bytes read if successful; -1 if failed
 */
ssize_t mynfs_read(int fd, void *buf, size_t size) {
  printf("mynfs_read issued\n");
  char com[64] = "mynfs_read ";
  char sfd[4], ssize[4];
  int n = 0;
  
  sprintf(sfd, "%d", fd);
  sprintf(ssize, "%d", (int)size);
  
  strcat(com, sfd);
  strcat(com, " ");
  strcat(com, ssize);
  printf("%s sent\n", com);
  if(write(sock, com, 1024) == -1) {
    mynfs_error = 6;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    mynfs_error = 1;
    return -1;
  }
  
  if(read(sock, &n, sizeof(int)) == -1) {
    mynfs_error = 7;
  }
  
  printf("%d received\n", n);
  
  if(read(sock, buf, n) == -1) {
    mynfs_error = 8;
  }
  
  printf("buf: %s\n", (char *)buf);

  return n;
}

/*
 * function: mynfs_write
 *
 * sends a request to write count number of bytes from buf to the server file
 * specified by fd
 *
 * fd - server file descriptor
 * buf - pointer to the buffer to write from
 * count - number of bytes to write
 *
 * returns: number of bytes written if successful; -1 if failed
 */
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
  if(write(sock, com, 1024)) {
    mynfs_error = 9;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    mynfs_error = 1;
    return -1;
  }
  
  printf("sent\n");

  if(write(sock, buf, count) == -1) {
    mynfs_error = 10;
  }
  if(read(sock, &res, sizeof(int)) == -1) {
    mynfs_error = 11;
  }
  return res;
}

/*
 * function: mynfs_lseek
 *
 * sends a request to lseek on the server file specified by fd to the offset according
 * to the directive whence
 *
 * fd - server file descriptor
 * offset - number of bytes to offset by
 * whence - directive SEEK_SET, SEEK_CUR or SEEK_END
 *
 * returns: offset location measured in bytes from the start of the file if successful;
 * -1 if failed
 */
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
  	mynfs_error = 12;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    mynfs_error = 1;
    return -1;
  }
  
  
  if(read(sock, &result, sizeof(int)) == -1) {
    mynfs_error = 13;
  	send_failure();
  } else {
  	send_success();
  }
  
  printf("lseek result = %d\n", result);  
  return result;
}

/*
 * function: mynfs_unlink
 *
 * sends a request to unlink the file specified by the path
 *
 * path - path to the server file
 *
 * returns: 0 if successful; -1 if failed
 */
int mynfs_unlink(char *path) {
	int response, result = -1;
  char com[64] = "mynfs_unlink ";
  strcat(com, path);
  
  if(write(sock, com, 1024) == -1) {
  	mynfs_error = 14;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    mynfs_error = 1;
    return -1;
  }
  
  
  if(read(sock, &response, sizeof(int)) == -1) {
  	mynfs_error = 15;
  }
  
  if(response == 1) result = 0;
  printf("response = %d\n", response);
  return result;
}

/*
 * function: mynfs_fstat
 *
 * sends a request to read file stats and saves them in buf
 *
 * fd - server file descriptor
 * buf - pointer to a stat structure
 *
 * returns: 0 if successful; -1 if failed
 */
int mynfs_fstat(int fd, struct stat *buf) {
  char sfd[4], com[64] = "mynfs_fstat ";
  
  sprintf(sfd, "%d", fd);
  strcat(com, sfd);
  if(write(sock, com, 1024) == -1) {
    mynfs_error = 16;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    mynfs_error = 1;
    return -1;
  }
  
  printf("mynfs_fstat issued\n");
  
  int res;
  if(read(sock, &res, sizeof(int)) == -1) {
    mynfs_error = 17;
  }
  if(!res) {
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
  
  return 0;
}

/*
 * function: mynfs_opendir
 *
 * sends a request to open the dir located in path
 *
 * path - path to the dir on the server
 *
 * returns: dir descriptor if successful; -1 if failed
 */
int mynfs_opendir(char *path) {
	int dd;
  char com[64] = "mynfs_opendir ";
  strcat(com, path);
  
  if(write(sock, com, 1024) == -1) {
  	mynfs_error = 18;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    mynfs_error = 1;
    return -1;
  }
  
  if(read(sock, &dd, sizeof(int)) == -1) {
  	mynfs_error = 19;
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

/*
 * function: mynfs_closedir
 *
 * sends a request to close the dir specified by dd
 *
 * dd - server directory descriptor
 *
 * returns: 0 if successful; -1 if failed
 */
int mynfs_closedir(int dd) {
	int response, result = 0;
  char sdd[4], com[64] = "mynfs_closedir ";
  
  sprintf(sdd, "%d", dd);
  strcat(com, sdd);
  
  if(write(sock, com, 1024) == -1) {
  	mynfs_error = 20;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    mynfs_error = 1;
    return -1;
  }
  
  if(read(sock, &response, sizeof(int)) == -1) {
  	mynfs_error = 21;
  }
  
  printf("response = %d\n", response);
  
  if(response == 0) {
  	result = -1;
  }
  
  remove_opened_dir(dd);
  printf("mynfs_closedir issued\n");
  return result;
}

/*
 * function: mynfs_readdir
 *
 * sends a request to read directory entries
 *
 * dd - server directory descriptor
 * buf - pointer to a char buffer
 *
 * returns: 0 if successful; -1 if failed
 */
int mynfs_readdir(int dd, char *buf) {
  char com[64] = "mynfs_readdir ";
  char sdd[4];
  
  sprintf(sdd, "%d", dd);
  strcat(com, sdd);
  if(write(sock, com, 1024) == -1) {
    mynfs_error = 22;
  }
  
  int resp;
  read(sock, &resp, sizeof(int));
  if(!resp) {
    mynfs_error = 1;
    return -1;
  }
  
  if(read(sock, buf, 1024) == -1) {
    mynfs_error = 23;
  }  
  send_success();
  
  if(!strcmp(buf, "")) {
    return -1;
  }
  
  printf("mynfs_readdir issued\n");
  return 1;
}
