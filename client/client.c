#include "client.h"

void help() {
  printf("Available commands:\n");
  printf("mynfs_open <host> <path> <flags> <mode>\n");
  printf("mynfs_read\n");
  printf("mynfs_write <path to the local file>\n");
  printf("mynfs_lseek <offset> <whence>\n");
  printf("mynfs_close\n");
  printf("mynfs_unlink <host> <path>\n");
  printf("mynfs_opendir <host> <path>\n");
  printf("mynfs_readdir\n");
  printf("mynfs_closedir\n");
  printf("mynfs_fstat\n");
}

int mynfs_open(char *arg) {
/*
send com+arg
recv response
*/
  char com[50] = "mynfs_open ";
  char buf[1024];
  strcat(com, arg);
  write(sock, com, 1024);
//  read(sock, buf, sizeof(buf));  // expecting success
//  write(sock, arg, 1024);
  printf("mynfs_open issued\n");
  return 1;
}

int mynfs_close(char *arg) {
  char com[50] = "mynfs_close ";
  strcat(com, arg);
  write(sock, com, 1024);
  printf("mynfs_close issued\n");
  return 1;
}

int mynfs_read(char *arg) {
/*
send com
recv buf to read
*/
  char com[64] = "mynfs_read ";
  char buf[1024];
  char *path, *fd, *size;
  int local_fd, n;
  n = 0;
  fd = strtok(arg, " ");
  path = strtok(NULL, " ");
  size = strtok(NULL, " ");
  
  local_fd = open(path, O_RDWR|O_CREAT, 00700);
  printf("%d\n", local_fd);
  if(local_fd == -1) {
    mynfs_error = 6;
    return 0;
  }
  
  strcat(com, fd);
  strcat(com, " ");
  strcat(com, size);
  printf("%s sent\n", com);
  write(sock, com, 1024);
  
  read(sock, &n, sizeof(int));
  printf("%d received\n", n);
  read(sock, buf, n);
  printf("buf: %s\n", buf);
  if(write(local_fd, buf, n-1) == -1) {
    printf("wot\n");
    mynfs_error = 8;
    return 0;
  }
  return 1;
}

int mynfs_write(char *arg) {
/*
send com
recv success
send buf to write
*/
  char com[64] = "mynfs_write ";
  char buf[1024];
  char *path, *fd, *sn;
  int local_fd, size, n;
  
  path = strtok(arg, " ");
  fd = strtok(NULL, " ");
  size = atoi(strtok(NULL, " "));
  
  local_fd = open(path, O_RDONLY);
  
  if(local_fd == -1) {
    mynfs_error = 7;
    return 0;
  }
  
  n = read(local_fd, buf, size);
  printf("%s", com);
  
  strcat(com, fd);
  strcat(com, " ");
  sprintf(sn, "%d", n);
  strcat(com, sn);
  
  printf("%s\n", com); //
  write(sock, com, sizeof(com));
  printf("sent\n"); //
  
  write(sock, buf, n);
  
  return 1;
}

int mynfs_lseek(char *arg) {
/*
send com+arg
recv result(?)
*/
  printf("mynfs_lseek issued\n");
  return 1;
}

int mynfs_unlink(char *arg) {
/*
send com+arg
recv response
*/
  printf("mynfs_unlink issued\n");
  return 1;
}

int mynfs_opendir(char *arg) {
/*
send com+arg
recv response
*/
  char com[50] = "mynfs_opendir ";
  char buf[1024];
  strcat(com, arg);
  write(sock, com, 1024);
  printf("mynfs_opendir issued\n");
  return 1;
}

int mynfs_closedir(char *arg) {
/*
send com
recv response
*/
  char com[50] = "mynfs_closedir ";
  char buf[1024];
  strcat(com, arg);
  write(sock, com, 1024);
  printf("mynfs_closedir issued\n");
  return 1;
}

int mynfs_readdir() {
/*
send com
recv buf
*/
  printf("mynfs_readdir issued\n");
  return 1;
}

int mynfs_fstat() {
/*
send com
recv buf with stats
*/
  printf("mynfs_fstat issued\n");
  return 1;
}

void init_client_socket(char *host, char *port) {
  struct sockaddr_in server;
  struct hostent *hp, *gethostbyname();
  char buf[1024], *check;

  /* Create socket */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1) {
    perror("opening stream socket");
    exit(1);
  }

/*  check = strtok(host, ".");
  if(check == NULL) {
*/
    /* Get the IP address from hostname */
/*    hp = gethostbyname(host);

    if(hp == (struct hostent *) 0) {
      fprintf(stderr, "%s: unknown host\n", host);
      exit(2);
    }
    memcpy((char *) &server.sin_addr, (char *) hp->h_addr, hp->h_length);
  } else {*/
    server.sin_addr.s_addr = inet_addr(host);
//  }
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(port));

  if(connect(sock, (struct sockaddr *) &server, sizeof server) == -1) {
    perror("connecting stream socket");
    exit(1);
  }

  printf("Type \"help\" to list available commands\n");
}

void client_exec() {
  while(1) {
    int n = 128;
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

    if(!strcmp(com, "help"))
      help();
    if(!strcmp(com, "mynfs_open"))
      res = mynfs_open(arg);
    if(!strcmp(com, "mynfs_read"))
      res = mynfs_read(arg);
    if(!strcmp(com, "mynfs_write"))
      res = mynfs_write(arg);
    if(!strcmp(com, "mynfs_lseek"))
      res = mynfs_lseek(arg);
    if(!strcmp(com, "mynfs_close"))
      res = mynfs_close(arg);
    if(!strcmp(com, "mynfs_unlink"))
      res = mynfs_unlink(arg);
    if(!strcmp(com, "mynfs_opendir"))
      res = mynfs_opendir(arg);
    if(!strcmp(com, "mynfs_readdir"))
      res = mynfs_readdir();
    if(!strcmp(com, "mynfs_closedir"))
      res = mynfs_closedir(arg);
    if(!strcmp(com, "mynfs_fstat"))
      res = mynfs_fstat();
    if(!strcmp(com, "close"))
      break;
  }
  close(sock);
}
