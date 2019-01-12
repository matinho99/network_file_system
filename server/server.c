#include "server.h"

int mynfs_open(char *host, char *path, int flags, int mode) {
  printf("mynfs_open %s %s %d %d\n", host, path, flags, mode);
}

int mynfs_close(int fd) {
  printf("mynfs_close %d\n", fd);
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

int mynfs_opendir(char *host, char *path) {
  printf("mynfs_opendir %s %s\n", host, path);
}

int mynfs_closedir(int dirfd) {
  printf("mynfs_closedir %d\n", dirfd);
}

int mynfs_readdir(int dirfd) {
  printf("mynfs_readdir %d\n", dirfd);
}

int mynfs_fstat(int fd) {
  printf("mynfs_fstat %d\n", fd);
}

void decode_message(char *message) {
  
}

void init_server_socket() {
  struct sockaddr_in addr;

  if((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("server_socket creation failed");
    exit(0);
  }

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);

  if(bind(server_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("server socket bind failed");
    exit(0);
  }
}

void server_exec() {
  int max_sock, addrlen, num_clients_connected = 0, i;
  char buffer[1024];
  struct sockaddr_in addr;
  fd_set readfds;
  addrlen = sizeof addr;
  listen(server_sock, 5);
  max_sock = server_sock;

  for(i = 0; i < MAX_CLIENTS; i++) {
    client_sockets[i] = 0;
  }

  while(1) {
    FD_ZERO(&readfds);     
    FD_SET(server_sock, &readfds);

    for(i = 0; i<MAX_CLIENTS; i++) {
      if(client_sockets[i] > 0)   
        FD_SET(client_sockets[i], &readfds);   
                 
      if(client_sockets[i] > max_sock)   
        max_sock = client_sockets[i];   
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

      printf("host connected on sock %d; client IP: %s\n", new_sock, inet_ntoa(addr.sin_addr));
      num_clients_connected++;

      for(i = 0; i < MAX_CLIENTS; i++) {   
        if(client_sockets[i] == 0) {
          client_sockets[i] = new_sock;
          break;
        }   
      }
    }

    for(i = 0; i < MAX_CLIENTS; i++) {
      int sock = client_sockets[i];

      if(FD_ISSET(sock, &readfds)) {
        int num_bytes_read = read(sock, buffer, sizeof(buffer));
        if(num_bytes_read > 0) {
	  printf("message received: %s\n", buffer);
	  decode_message(buffer);
	  char *success = "OK";
	  write(sock, success, sizeof success);
	  char *buffer_parts = strtok(buffer, " ");

	  while(buffer_parts != NULL) {
	    printf("%s\n", buffer_parts);
	    buffer_parts = strtok(NULL, " ");
	  }
        } else if(num_bytes_read == 0) {
          printf("host disconnected\n");
          //send_success(sock);
	  client_sockets[i] = 0;
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
  }

  close(server_sock);
}
