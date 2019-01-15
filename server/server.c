#include "server.h"

int mynfs_open(struct client_info ci, char *path, int flags, int mode) {
  printf("mynfs_open %s %s %d %d\n", ci.ip, path, flags, mode);
  int fd;

  /* check if file exists */
  /*if(access(path, F_OK) != -1) {  // file exists
    if(mode != 0)
      access 
  } else {  // file doesn't exist, O_CREAT flag
    if(flags == O_CREAT)
      
  } */
  if(mode == O_CREAT)
    fd = open(path, flags, mode);
  else
    fd = open(path, flags);

  if(fd == -1)
    mynfs_error = 1;

  return fd;
}

int mynfs_close(int fd) {
  printf("mynfs_close %d\n", fd);
  int result;
  
  if((result = close(fd)) == -1) {
    mynfs_error = 2;
  }

  return result;
}

int mynfs_read(int fd, void *buf, int size) {
  printf("mynfs_read %d buf %d\n", fd, size);


}

int mynfs_write(int fd, void *buf, int size) {
  printf("mynfs_write %d buf %d\n", fd, size);
}

int mynfs_lseek(int fd, int offset, int whence) {
  printf("mynfs_lseek %d %d %d\n", fd, offset, whence);
}

int mynfs_unlink(int fd) {
  printf("mynfs_unlink %d\n", fd);
}

int mynfs_fstat(int fd) {
  printf("mynfs_fstat %d\n", fd);
}

int mynfs_opendir(struct client_info ci, char *path) {
  printf("mynfs_opendir %s %s\n", ci.ip, path);
  DIR *dir_p;  // might need to be global
  int dd = -1;
  dir_p = opendir(path);

  if(dir_p == NULL) {
    mynfs_error = 3;
  } else {
    dd = dirfd(dir_p);
  }

  return dd;
}

int mynfs_closedir(int dd) {
  printf("mynfs_closedir %d\n", dd);
  DIR *dir_p = fdopendir(dd);
  int result;
/*
  for(int i = 0; i < opened_dirs_arr.num_opened_dirs; i++) {
    if(opened_dirs_arr.opened_dirs[i].dir_descriptor == dd) {
      dir_p = opened_dirs_arr.opened_dirs[i].dir_pointer;
      break;
    }
  }*/

  if((result = closedir(dir_p)) == -1) {
    mynfs_error = 4;
  }

  return result;
}

int mynfs_readdir(int dd) {
  printf("mynfs_readdir %d\n", dirfd);
}

void exec_operation(char *message, struct client_info ci) {
  char *op = strtok(message, " ");
  int fd = 0, dd;

  if(!strcmp(op, "mynfs_open")) {
    char *filepath = strtok(NULL, " ");
    int flags = atoi(strtok(NULL, " "));
    int mode = atoi(strtok(NULL, " "));

    if(has_access_to_file(ci, filepath, flags) && !has_opened_file_by_path(ci, filepath)) {
      //send_success
      if((fd = mynfs_open(ci, filepath, flags, mode)) != -1) {
        add_opened_file(ci, fd, filepath, flags);
      }

      printf("mynfs_open fd: %d", fd);
    }
  } else if(!strcmp(op, "mynfs_close")) {
    fd = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd)) {
      int res;

      if((res = mynfs_close(fd)) != -1) {
	remove_opened_file(ci, fd);
      }
    }
  } else if(!strcmp(op, "mynfs_read")) {
    fd = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd) && has_read_access(ci, fd)) {
      int res;
      //mynfs_read(fd);
    }
  } else if(!strcmp(op, "mynfs_write")) {
    fd = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd) && has_read_access(ci, fd)) {
      int res;
      //mynfs_write(fd);
    }
  } else if(!strcmp(op, "mynfs_lseek")) {
    fd = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd)) {
      int res;
      mynfs_close(fd);
    }
  } else if(!strcmp(op, "mynfs_unlink")) {
    fd = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd) && has_write_access(ci, fd)) {
      int res;
      mynfs_close(fd);
    }
  } else if(!strcmp(op, "mynfs_fstat")) {
    fd = atoi(strtok(NULL, " "));

    if(has_opened_file(ci, fd)) {
      int res;
      mynfs_close(fd);
    }
  } else if(!strcmp(op, "mynfs_opendir")) {
    char *dirpath = strtok(NULL, " ");

    if(has_access_to_dir(ci, dirpath) && !has_opened_dir_by_path(ci, dirpath)) {
      if((dd = mynfs_opendir(ci, dirpath)) != -1) {
	add_opened_dir(ci, dd, dirpath);
      }
    }
  } else if(!strcmp(op, "mynfs_closedir")) {
    dd = atoi(strtok(NULL, " "));

    if(has_opened_dir(ci, dd)) {
      if(mynfs_closedir(dd) == 0) {
        remove_opened_dir(ci, dd);
      }
    }
  } else if(!strcmp(op, "mynfs_readdir")) {
    dd = atoi(strtok(NULL, " "));

    if(has_opened_dir(ci, dd)) {
      mynfs_readdir(dd);
    }
  } else {
    send_failure(ci);
  }
}

void server_exec() {
  int max_sock, addrlen, num_clients_connected = 0, i;
  char buffer[1024];
  struct sockaddr_in addr;
  fd_set readfds;
  addrlen = sizeof addr;
  init_server_socket(&server_sock);
  load_client_accesses();
  listen(server_sock, 5);
  max_sock = server_sock;

  while(1) {
    FD_ZERO(&readfds);     
    FD_SET(server_sock, &readfds);

    for(i = 0; i<MAX_CLIENTS; i++) {
      if(client_sockets[i].sock > 0)   
        FD_SET(client_sockets[i].sock, &readfds);   
                 
      if(client_sockets[i].sock > max_sock)   
        max_sock = client_sockets[i].sock;   
    }

    if(select(max_sock+1, &readfds, NULL, NULL, NULL) < 0) {
      printf("select failed");
    }

    if(FD_ISSET(server_sock, &readfds)) {
      int new_sock;

      if((new_sock = accept(server_sock, (struct sockaddr *)&addr, (socklen_t*)&addrlen)) < 0) {   
        perror("accept failed");   
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
	  send_success(client_sockets[i]);
        } else if(num_bytes_read == 0) {
          printf("host disconnected\n");
	  client_sockets[i].sock = 0;
          close(sock);

          if(num_clients_connected == 1) {
	    num_clients_connected = -1; 
	  } else num_clients_connected--;          
        } else if(num_bytes_read < 0) {
	  printf("something went wrong\n");
	}
      }
    }

    printf("number of clients connected = %d\n", num_clients_connected);
    if(num_clients_connected == -1) break;

    for(int i=0; i<opened_files_arr.num_opened_files; i++) {
      printf("Opened file: %d %s %s %d; number of opened files: %d\n", opened_files_arr.opened_files[i].file_descriptor, opened_files_arr.opened_files[i].filepath, 
        opened_files_arr.opened_files[i].client_ip, opened_files_arr.opened_files[i].flags, opened_files_arr.num_opened_files);
    }

    for(int i=0; i<opened_dirs_arr.num_opened_dirs; i++) {
      printf("Opened dir: %d %s; number of opened files: %d\n", opened_dirs_arr.opened_dirs[i].dir_descriptor, opened_dirs_arr.opened_dirs[i].client_ip, 
	opened_dirs_arr.num_opened_dirs);
    }
  }

  close(server_sock);
  if(client_accesses_arr.client_accesses != NULL) free(client_accesses_arr.client_accesses);
}
