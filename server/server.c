#include "server.h"

/*
 * function: mynfs_open
 *
 * opens the specified file with appropriate flags and sends a descriptor to client
 *
 * ci - client info storing host's ip and socket
 * path - path to file to open
 * flags - open options like O_RDONLY, O_WRONLY
 * mode - additional options if O_CREAT is specified in flags
 *
 * returns: file descriptor if successful; -1 if failed
 */
int mynfs_open(struct client_info ci, char *path, int flags, int mode) {
  printf("mynfs_open %s %s %d %d\n", ci.ip, path, flags, mode);
  int fd, response;

  if(flags == O_CREAT|O_RDONLY || flags == O_CREAT|O_WRONLY || flags == O_CREAT|O_RDWR) {
    fd = open(path, flags, 00700);
  } else {
    fd = open(path, flags);
  }

  if(fd == -1) {
    mynfs_error = 1;
  }
  
  if(write(ci.sock, &fd, sizeof(int)) == -1) {
    mynfs_error = 2;
  }
  
  if(read(ci.sock, &response, sizeof(int)) == -1) {
    mynfs_error = 3;
  }
  
  printf("mynfs_error = %d\n", mynfs_error);
  return fd;
}

/*
 * function: mynfs_close
 *
 * closes file specified by the file descriptor
 *
 * fd - file descriptor
 *
 * returns: 0 if successful; -1 if failed
 */
int mynfs_close(int fd) {
  printf("mynfs_close %d\n", fd);
  int result;
  
  if((result = close(fd)) == -1) {
    mynfs_error = 4;
  }

  return result;
}

/*
 * function: mynfs_read
 *
 * reads number of bytes specified by size from file to buffer
 *
 * fd - file descriptor
 * buf - buffer for data from file
 * size - number of bytes to read from file
 *
 * returns: number of bytes read; -1 if failed
 */
int mynfs_read(int fd, void *buf, int size) {
  printf("mynfs_read %d buf %d\n", fd, size);
	int result;
  
  if((result = read(fd, buf, size)) == -1) {
  	mynfs_error = 5;
  }
  
  return result;
}

/*
 * function: mynfs_write
 *
 * writes number of bytes specified by size from buffer to file
 *
 * fd - file descriptor
 * buf - buffer with data for file
 * size - number of bytes to write to file
 *
 * returns: number of bytes written; -1 if failed
 */
int mynfs_write(int fd, void *buf, int size) {
  printf("mynfs_write %d buf %d\n", fd, size);
  int result;
  
  if((result = write(fd, buf, size)) == -1) {
  	mynfs_error = 6;
  }
  
  return result;
}

/*
 * function: mynfs_lseek
 *
 * moves the pointer inside of file stream by offset
 *
 * fd - file descriptor
 * offset - number of bytes to move pointer by
 * whence - position from which the pointer is moved: SEEK_SET - beginning, 
 *    SEEK_END - end, SEEK_CUR - current position
 *
 * returns: position of pointer in file; -1 if failed
 */
int mynfs_lseek(int fd, int offset, int whence) {
  printf("mynfs_lseek %d %d %d\n", fd, offset, whence);
  int result;
  
  if((result = lseek(fd, offset, whence)) == -1) {
    mynfs_error = 7;
  }

  return result;
}

/*
 * function: mynfs_unlink
 *
 * deletes file specified by path
 *
 * path - file path
 *
 * returns: 0 if successful; -1 if failed
 */
int mynfs_unlink(char *path) {
  printf("mynfs_unlink %s\n", path);
  int result;
  
  if((result = unlink(path)) == -1) {
    mynfs_error = 8;
  }

  return result;
}

/*
 * function: mynfs_fstat
 *
 * sends information from stat structure for specified file
 *
 * ci - client information
 * fd - file descriptor
 *
 * returns: 0 if successful; -1 if failed
 */
int mynfs_fstat(struct client_info ci, int fd) {
  printf("mynfs_fstat %d\n", fd);
  int result;
  struct stat buf;
  
  if((result = fstat(fd, &buf)) == -1) {
    mynfs_error = 9;
  } else {
    printf("st_dev %d; st_ino %d; st_uid %d; st_size %d; st_atime %d\n", buf.st_dev, buf.st_ino, 
      buf.st_uid, buf.st_size, buf.st_atime);
  }
  
  write(ci.sock, &buf.st_dev, sizeof(dev_t));
  write(ci.sock, &buf.st_ino, sizeof(ino_t));
  write(ci.sock, &buf.st_mode, sizeof(mode_t));
  write(ci.sock, &buf.st_nlink, sizeof(nlink_t));
  write(ci.sock, &buf.st_uid, sizeof(uid_t));
  write(ci.sock, &buf.st_gid, sizeof(gid_t));
  write(ci.sock, &buf.st_rdev, sizeof(dev_t));
  write(ci.sock, &buf.st_size, sizeof(off_t));
  write(ci.sock, &buf.st_blksize, sizeof(blksize_t));
  write(ci.sock, &buf.st_blocks, sizeof(blkcnt_t));
  write(ci.sock, &buf.st_atime, sizeof(time_t));
  write(ci.sock, &buf.st_mtime, sizeof(time_t));
  write(ci.sock, &buf.st_ctime, sizeof(time_t));
  return result;
}

/*
 * function: mynfs_opendir
 *
 * opens directory specified by path
 *
 * ci - client information
 * path - path to directory
 *
 * returns: directory descriptor if successful; -1 if failed
 */
int mynfs_opendir(struct client_info ci, char *path) {
  printf("mynfs_opendir %s %s\n", ci.ip, path);
  DIR *dir_p;  // might need to be global
  char buf[1024];
  int response, dd = -1;
  dir_p = opendir(path);

  if(dir_p == NULL) {
    mynfs_error = 10;
  } else {
    dd = dirfd(dir_p);
  }
  
  if(write(ci.sock, &dd, sizeof(int)) == -1) {
    mynfs_error = 2;
  }
  
  if(read(ci.sock, &response, sizeof(int)) == -1) {
    mynfs_error = 3;
  }

  return dd;
}

/*
 * function: mynfs_closedir
 *
 * closes directory specified by the directory descriptor
 *
 * dd - directory descriptor
 *
 * returns: 0 if successful; -1 if failed
 */
int mynfs_closedir(int dd) {
  printf("mynfs_closedir %d\n", dd);
  DIR *dir_p = fdopendir(dd);
  int result;

  if((result = closedir(dir_p)) == -1) {
    mynfs_error = 11;
  }

  return result;
}

/*
 * function: mynfs_readdir
 *
 * sends directory entries to client
 *
 * ci - client information
 * dd - directory descriptor
 *
 * returns: 0 if successful; -1 if failed
 */
int mynfs_readdir(struct client_info ci, int dd) {
  printf("mynfs_readdir %d\n", dd);
  char buf[1024];
  int response, result = 0;
  DIR *dir_p = fdopendir(dd);
  struct dirent *de;
  errno = 0;
  strcpy(buf, "");
 	
  while((de = readdir(dir_p)) != NULL) {
    strcat(buf, de->d_name);
    strcat(buf, "/");
  }
  
  if(errno != 0) {
    mynfs_error = 12;
    strcpy(buf, "");
    result = -1;
  }
  
  rewinddir(dir_p);
  printf("buf after readdir: %s\n", buf);
  
  if(write(ci.sock, buf, 1024) == -1) {
    mynfs_error = 2;
  }
  
  if(read(ci.sock, &response, sizeof(int)) == -1) {
		mynfs_error = 3;
	}
  
  return result;
}

/*
 * function: exec_operation
 *
 * executes operations specified in message from client
 *
 * message - message from client containing operation type and arguments
 * ci - client information
 */
void exec_operation(char *message, struct client_info ci) {
  char *op = strtok(message, " ");
  char buf[1024];
  int fd, dd;

  if(!strcmp(op, "mynfs_open")) {
    char *filepath = strtok(NULL, " ");
    int flags, mode;
    char *tmp = strtok(NULL, " ");
    if(tmp != NULL) flags = atoi(tmp);
    tmp = strtok(NULL, " ");
    if(tmp != NULL) mode = atoi(tmp);

    if(has_access_to_file(ci, filepath, flags) && !has_opened_file_by_path(ci, filepath)) {
      send_success(ci);
      
      if((fd = mynfs_open(ci, filepath, flags, mode)) != -1 && mynfs_error == 0) {
        add_opened_file(ci, fd, filepath, flags);
      }
    } else {
      send_failure(ci);
    }
  } else if(!strcmp(op, "mynfs_close")) {
    fd = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd)) {
      send_success(ci);
      int res = mynfs_close(fd);
      
      if(write(ci.sock, &res, sizeof(int)) == -1) {
        mynfs_error = 13;
      }
      
      if(res == 0) {
	      remove_opened_file(ci, fd);
      }
    } else {
      send_failure(ci);
    }
  } else if(!strcmp(op, "mynfs_read")) {
    int size;
    fd = atoi(strtok(NULL, " "));
    size = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd) && has_read_access(ci, fd)) {
      send_success(ci);
      int res;

			if((res = mynfs_read(fd, buf, size)) >= 0) {
				if(write(ci.sock, &res, sizeof(int)) == -1) {
					mynfs_error = 14;
				}
		
				if(write(ci.sock, buf, res) == -1) {
			 		mynfs_error = 15;
				}
			}
    } else {
      send_failure(ci);
    }
  } else if(!strcmp(op, "mynfs_write")) {
  	int size;
    fd = atoi(strtok(NULL, " "));
    size = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd) && has_write_access(ci, fd)) {
      send_success(ci);
      int res;
      char buf[size];
      
      if(read(ci.sock, buf, size) == -1) {
      	mynfs_error = 16;
      }
      
      if((res = mynfs_write(fd, buf, size)) != -1) {
      	printf("%d\n%d\n", res, size);
      	
      	if(write(ci.sock, &res, sizeof(int)) == -1) {
      	  mynfs_error = 17;
      	}
      }
    } else {
      send_failure(ci);
    }
  } else if(!strcmp(op, "mynfs_lseek")) {
    fd = atoi(strtok(NULL, " "));
    int offset = atoi(strtok(NULL, " "));
    int whence = atoi(strtok(NULL, " "));
    int result, response;

    if(has_opened_file(ci, fd)) {
      send_success(ci);
      
      if((result = mynfs_lseek(fd, offset, whence)) != -1) {  
        if(write(ci.sock, &result, sizeof(int)) == -1) {
        	mynfs_error = 18;
        }
        
      	if(read(ci.sock, &response, sizeof(int)) == -1) {
      		mynfs_error = 3;
      	}
      }
    } else {
      send_failure(ci);
    }
  } else if(!strcmp(op, "mynfs_unlink")) {
    char *filepath = strtok(NULL, " ");
    
    if(has_access_to_file(ci, filepath, O_WRONLY)) {
      send_success(ci);
      int res = mynfs_unlink(filepath);
      
      if(write(ci.sock, &res, sizeof(int)) == -1) {
        mynfs_error = 19;
      }
    } else {
      send_failure(ci);
    }
  } else if(!strcmp(op, "mynfs_fstat")) {
    fd = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd)) {
      send_success(ci);
      mynfs_fstat(ci, fd);
    } else {
      send_failure(ci);
    }
  } else if(!strcmp(op, "mynfs_opendir")) {
    char *dirpath = strtok(NULL, " ");

    if(has_access_to_dir(ci, dirpath) && !has_opened_dir_by_path(ci, dirpath)) {
      send_success(ci);
    
      if((dd = mynfs_opendir(ci, dirpath)) != -1 && mynfs_error == 0) {
	      add_opened_dir(ci, dd, dirpath);
      }
    } else {
      send_failure(ci);
    }
  } else if(!strcmp(op, "mynfs_closedir")) {
    dd = atoi(strtok(NULL, " "));

    if(has_opened_dir(ci, dd)) {
      send_success(ci);
      int res = mynfs_closedir(dd);
      
      if(write(ci.sock, &res, sizeof(int)) == -1) {
        mynfs_error = 20;
      }
      
      if(res == 0) {
	      remove_opened_dir(ci, dd);
      }
    } else {
      send_failure(ci);
    }
  } else if(!strcmp(op, "mynfs_readdir")) {
    dd = atoi(strtok(NULL, " "));

    if(has_opened_dir(ci, dd)) {
      send_success(ci);
      mynfs_readdir(ci, dd);
    } else {
      send_failure(ci);
    }
  }
}

/*
 * function: server_exec
 *
 * server execution main function; multiple clients management; printing opened files and directories
 */
void server_exec() {
  int max_sock, addrlen, num_clients_connected = 0, i, select_val;
  char buffer[1024];
  struct sockaddr_in addr;
  fd_set readfds;
  struct timeval to;
  addrlen = sizeof addr;
  init_server_socket(&server_sock);
  load_client_accesses();
  listen(server_sock, 5);
  max_sock = server_sock;

  while(1) {
    FD_ZERO(&readfds);     
    FD_SET(server_sock, &readfds);
    to.tv_sec = TIMEOUT_SEC;
	  to.tv_usec = 0;

    for(i = 0; i<MAX_CLIENTS; i++) {
      if(client_sockets[i].sock > 0)   
        FD_SET(client_sockets[i].sock, &readfds);   
                 
      if(client_sockets[i].sock > max_sock)   
        max_sock = client_sockets[i].sock;   
    }

    if((select_val = select(max_sock+1, &readfds, NULL, NULL, &to)) < 0) {
      perror("select failed\n");
      break;
    } else if (select_val == 0) {
    	printf("timeout\n");
    	break;
    }

    if(FD_ISSET(server_sock, &readfds)) {
      int new_sock;

      if((new_sock = accept(server_sock, (struct sockaddr *)&addr, (socklen_t*)&addrlen)) < 0) {   
        perror("accept failed");  
        server_close; 
        exit(0); 
      }

      for(i = 0; i < MAX_CLIENTS; i++) {
        if(client_sockets[i].sock == 0) {
          client_sockets[i].sock = new_sock;
	        strcpy(client_sockets[i].ip, inet_ntoa(addr.sin_addr));
	        printf("host connected on sock %d; client IP: %s\n", client_sockets[i].sock, client_sockets[i].ip);
          break;
        }   
      }

      num_clients_connected++;
    }

    for(i = 0; i < MAX_CLIENTS; i++) {
      int sock = client_sockets[i].sock;

      if(FD_ISSET(sock, &readfds)) {
        int num_bytes_read = read(sock, buffer, sizeof(buffer));

        if(num_bytes_read > 0) {
	        printf("message received: %s\n", buffer);
	        exec_operation(buffer, client_sockets[i]);
	        //send_success(client_sockets[i]);
        } else if(num_bytes_read == 0) {
          printf("host disconnected\n");
	        client_sockets[i].sock = 0;
          close(sock);
					num_clients_connected--;          
        } else if(num_bytes_read < 0) {
	          printf("something went wrong\n");
	      }
      }
    }

    printf("number of clients connected = %d\n", num_clients_connected);

    for(int i=0; i<opened_files_arr.num_opened_files; i++) {
      printf("Opened file: %d %s %s %d; number of opened files: %d\n", opened_files_arr.opened_files[i].file_descriptor, opened_files_arr.opened_files[i].filepath, 
        opened_files_arr.opened_files[i].client_ip, opened_files_arr.opened_files[i].flags, opened_files_arr.num_opened_files);
    }

    for(int i=0; i<opened_dirs_arr.num_opened_dirs; i++) {
      printf("Opened dir: %d %s; number of opened files: %d\n", opened_dirs_arr.opened_dirs[i].dir_descriptor, opened_dirs_arr.opened_dirs[i].client_ip, 
	      opened_dirs_arr.num_opened_dirs);
    }
    
    if(mynfs_error != 0) {
      print_error();
      mynfs_error = 0;
    }
  }

  server_close();
}

/*
 * function: server_close
 *
 * server closing functions; closing sockets, freeing memory, closing files and directories
 */
void server_close() {
	int i;
  close(server_sock);
  
  for(i = 0; i < MAX_CLIENTS; i++) {
    if(client_sockets[i].sock != 0) {
    	printf("closing client socket %d\n", client_sockets[i].sock);
      close(client_sockets[i].sock);
    }
  }
  
  for(i = 0; i < opened_files_arr.num_opened_files; i++) {
   	printf("closing file %d\n", opened_files_arr.opened_files[i].file_descriptor);
  	close(opened_files_arr.opened_files[i].file_descriptor);
  }
  
  for(i = 0; i < opened_dirs_arr.num_opened_dirs; i++) {
  	printf("closing dir %d\n", opened_dirs_arr.opened_dirs[i].dir_descriptor);
  	DIR *dir_p = fdopendir(opened_dirs_arr.opened_dirs[i].dir_descriptor);
  	closedir(dir_p);
  }
  
  if(client_accesses_arr.client_accesses != NULL) free(client_accesses_arr.client_accesses);
}
