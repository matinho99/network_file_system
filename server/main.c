#include "server.h"

#define PORT 8080

int main(int argc, char **argv) {
  int server_sock, client_sockets[10], addrlen, max_sock, new_sock;
  struct sockaddr_in addr;
  char buffer[1025];
  fd_set readfds;

  if((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("server_socket creation failed");
    exit(0);
  }

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);
  addrlen = sizeof(addr);

  if(bind(server_sock, (struct sockaddr_in *)addr, sizeof(addr)) < 0) {
    perror("server socket bind failed");
    exit(0);
  }

  listen(server_sock, 5);
  FD_ZERO(&readfds);     
  FD_SET(server_sock, &readfds);   
  max_sock = server_sock;   
  
  for ( i = 0 ; i < max_clients ; i++) {   
    int sock = client_sockets[i];   
    
    if(sock > 0)   
      FD_SET(sock, &readfds);   
                 
    if(sock > max_sock)   
      max_sock = sd;   
  }

  while(1) {
    if(select( max_sock + 1 , &readfds , NULL , NULL , NULL) < 0) {
      printf("select failed");
    }

    if (FD_ISSET(server_sock, &readfds)) {   
      if ((new_sock = accept(server_sock, (struct sockaddr *)&addr, (socklen_t*)&addrlen)) < 0) {   
        perror("accept failed");   
        exit(EXIT_FAILURE); 
      }

      for (i = 0; i < max_clients; i++) {   
        if( client_socket[i] == 0 ) {
          client_socket[i] = new_socket;  
          break;   
        }   
      }
    }
    
    for (i = 0; i < max_clients; i++) {   
      int sock = client_socket[i];
               
      if (FD_ISSET(sock, &readfds)) {    
        if (read( sd , buffer, 1024) == 0) {   
          printf("message receive, do something");
        }
      }
    }
    break;
  }
  return 0;
}
