#include "server.h"

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
  
  printf("response = %d;\n", response);
  
  if(response == 0) {
    mynfs_error = 4;
  }
  
  printf("mynfs_error = %d\n", mynfs_error);
  return fd;
}

int mynfs_close(int fd) {
  printf("mynfs_close %d\n", fd);
  int result;
  
  if((result = close(fd)) == -1) {
    mynfs_error = 5;
  }

  return result;
}

int mynfs_read(int fd, void *buf, int size) {
  printf("mynfs_read %d buf %d\n", fd, size);
	int result;
  
  if((result = read(fd, buf, size)) == -1) {
  	mynfs_error = 6;
  }
  
  return result;
}

int mynfs_write(int fd, void *buf, int size) {
  printf("mynfs_write %d buf %d\n", fd, size);
  int result;
  
  if((result = write(fd, buf, size)) == -1) {
  	mynfs_error = 7;
  }
  
  return result;
}

int mynfs_lseek(int fd, int offset, int whence) {
  printf("mynfs_lseek %d %d %d\n", fd, offset, whence);
  int result;
  
  if((result = lseek(fd, offset, whence)) == -1) {
    mynfs_error = 8;
  }

  return result;
}

int mynfs_unlink(char *path) {
  printf("mynfs_unlink %s\n", path);
  int result;
  
  if((result = unlink(path)) == -1) {
    mynfs_error = 9;
  }

  return result;
}

int mynfs_fstat(int fd) {
  printf("mynfs_fstat %d\n", fd);
  int result;
  struct stat buf;
  
  if((result = fstat(fd, &buf)) == -1) {
    mynfs_error = 10;
  } else {
    printf("st_dev %d; st_ino %d; st_uid %d; st_size %d; st_atime %d\n", buf.st_dev, buf.st_ino, 
      buf.st_uid, buf.st_size, buf.st_atime);
  }
  
  return result;
}

int mynfs_opendir(struct client_info ci, char *path) {
  printf("mynfs_opendir %s %s\n", ci.ip, path);
  DIR *dir_p;  // might need to be global
  char buf[1024];
  int response, dd = -1;
  dir_p = opendir(path);

  if(dir_p == NULL) {
    mynfs_error = 11;
  } else {
    dd = dirfd(dir_p);
  }
  
  if(write(ci.sock, &dd, sizeof(int)) == -1) {
    mynfs_error = 2;
  }
  
  if(read(ci.sock, &response, sizeof(int)) == -1) {
    mynfs_error = 3;
  }
  
  printf("response = %d;\n", response);
  
  if(response == 0) {
    mynfs_error = 4;
  }

  return dd;
}

int mynfs_closedir(int dd) {
  printf("mynfs_closedir %d\n", dd);
  DIR *dir_p = fdopendir(dd);
  int result;

  if((result = closedir(dir_p)) == -1) {
    mynfs_error = 4;
  }

  return result;
}

char *mynfs_readdir(int dd) {
  printf("mynfs_readdir %d\n", dd);
  char result[1024];
  DIR *dir_p = fdopendir(dd);
  struct dirent *de;
  errno = 0;
 	strcpy(result, "");
 	
  while((de = readdir(dir_p)) != NULL) {
    strcat(result, de->d_name);
    strcat(result, "/");
  }
  
  if(errno != 0) {
    mynfs_error = 8;
  }
  
  rewinddir(dir_p);
  printf("result after readdir: %s\n", result);
  return result;
}

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
      //send_success
      if((fd = mynfs_open(ci, filepath, flags, mode)) != -1 && mynfs_error == 0) {
        add_opened_file(ci, fd, filepath, flags);
      } else {
//        get_error();
      }
    } else {
    	
    }
  } else if(!strcmp(op, "mynfs_close")) {
    fd = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd)) {
      int res;

      if((res = mynfs_close(fd)) == 0) {
	      remove_opened_file(ci, fd);
	      send_success(ci);
      } else {
      	send_failure(ci);
      }
    }
  } else if(!strcmp(op, "mynfs_read")) {
    int size;
    fd = atoi(strtok(NULL, " "));
    size = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd) && has_read_access(ci, fd)) {
      int res;

			if((res = mynfs_read(fd, buf, size))) {
				if(write(ci.sock, &res, sizeof(int)) == -1) {
					//mynfs_error = ;
				}
		
				if(write(ci.sock, buf, res) == -1) {
			 		//mynfs_error = ;
				}
			}
    }
  } else if(!strcmp(op, "mynfs_write")) {
  	int size;
    fd = atoi(strtok(NULL, " "));
    size = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd) && has_write_access(ci, fd)) {
      int res;
      char buf[size];
      
      if(read(ci.sock, buf, size) == -1) {
      	//mynfs_error
      }
      
      if((res = mynfs_write(fd, buf, size)) != -1) {
      	printf("%d\n%d\n", res, size);
      }
      //mynfs_write(fd);
    }
  } else if(!strcmp(op, "mynfs_lseek")) {
    fd = atoi(strtok(NULL, " "));
    int offset = atoi(strtok(NULL, " "));
    int whence = atoi(strtok(NULL, " "));
    int result, response;

    if(has_opened_file(ci, fd)) {
      if((result = mynfs_lseek(fd, offset, whence)) != -1) {  
        if(write(ci.sock, &result, sizeof(int)) == -1) {
//        	mynfs_error = ;
        }
        
      	if(read(ci.sock, &response, sizeof(int)) == -1) {
      		//mynfs_error = ;
      	}
      	
      	if(response == 0) {
      		//mynfs_error = ;
      	} else {
      		printf("lseek OK\n");
      	}
      }
    }
  } else if(!strcmp(op, "mynfs_unlink")) {
    char *filepath = strtok(NULL, " ");
    if(has_access_to_file(ci, filepath, O_WRONLY)) {
      if(mynfs_unlink(filepath) == 0) {
	      send_success(ci);
      }
    }
  } else if(!strcmp(op, "mynfs_fstat")) {
    fd = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd)) {
      if(mynfs_fstat(fd) != -1) {
        send_success(ci);
      }
    }
  } else if(!strcmp(op, "mynfs_opendir")) {
    char *dirpath = strtok(NULL, " ");

    if(has_access_to_dir(ci, dirpath) && !has_opened_dir_by_path(ci, dirpath)) {
      if((dd = mynfs_opendir(ci, dirpath)) != -1 && mynfs_error == 0) {
	      add_opened_dir(ci, dd, dirpath);
      } else {
//        get_error();
      }
    }
  } else if(!strcmp(op, "mynfs_closedir")) {
    dd = atoi(strtok(NULL, " "));

    if(has_opened_dir(ci, dd)) {
      if(mynfs_closedir(dd) == 0) {
        remove_opened_dir(ci, dd);
        send_success(ci);
      } else {
      	send_failure(ci);
      }
    }
  } else if(!strcmp(op, "mynfs_readdir")) {
    dd = atoi(strtok(NULL, " "));

    if(has_opened_dir(ci, dd)) {
      mynfs_readdir(dd);
    }
  }
}

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
  }

  server_close();
}

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
