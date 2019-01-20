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

int mynfs_open(char *arg) {
  char com[50] = "mynfs_open ";
  char buf[1024], *path;
  int fd, flags,success = 1;
  strcat(com, arg);

  if(write(sock, com, 1024) == -1) {
  	//mynfs_error = 1;
  }
  
  if(read(sock, &fd, sizeof(int)) == -1) {
  	//mynfs_error = 2;
  }
  
  printf("fd %d\n", fd);
  
  if(fd != -1) {
  	write(sock, &success, sizeof(int));
  	path = strtok(arg, " ");
  	flags = atoi(strtok(arg, " "));
  	add_opened_file(fd, path, flags);
  }
  
  printf("mynfs_open issued\n");
  return fd;
}

int mynfs_close(char *arg) {
  char com[50] = "mynfs_close ";
  strcat(com, arg);
  int response, fd, result = 0;
  
  if(write(sock, com, 1024) == -1) {
  	//mynfs_error = 3;
  }
  
  if(read(sock, &response, sizeof(int)) == -1) {
  	//mynfs_error = 4;
  }
  
  printf("response = %d\n", response);
  
  if(response == 0) {
  	result = -1;
  	//mynfs_error = 5;
  }
  
  fd = atoi(arg);
  remove_opened_file(fd);
  printf("mynfs_close issued\n");
  return result;
}

int mynfs_read(char *arg) {
  printf("mynfs_read issued\n");
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
  
  if(write(local_fd, buf, n) == -1) {
    printf("wot\n");
    mynfs_error = 8;
    return 0;
  }
  
  return 1;
}

int mynfs_write(char *arg) {
  char com[64] = "mynfs_write ";
  char buf[1024];
  char *path, *fd, sn[1024];
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
  printf("%s\n", buf);
  
  strcat(com, fd);
  strcat(com, " ");
  sprintf(sn, "%d", n);
  strcat(com, sn);
  printf("%s\n", com);
  write(sock, com, 1024);
  printf("sent\n");

  write(sock, buf, n);
  return 1;
}

int mynfs_lseek(char *arg) {
	printf("mynfs_lseek issued\n");
	int result, response = 0;
  char com[50] = "mynfs_lseek ";
  strcat(com, arg);
  
  if(write(sock, com, 1024) == -1) {
  	//mynfs_error = ;
  }
  
  if(read(sock, &result, sizeof(int)) == -1) {
  	//mynfs_error = ;
  } else {
  	response = 1;
  }
  
  if(write(sock, &response, sizeof(int)) == -1) {
  	//mynfs_error = ;
  }
  
  printf("lseek result = %d\n", result);  
  return result;
}

int mynfs_unlink(char *arg) {
	int response, result = -1;
  char com[50] = "mynfs_unlink ";
  strcat(com, arg);
  
  if(write(sock, com, 1024) == -1) {
  	//mynfs_error = ;
  }
  
  if(read(sock, &response, sizeof(int)) == -1) {
  	//mynfs_error = ;
  }
  
  if(response == 1) result = 0;
  printf("response = %d\n", response);
  return result;
}

int mynfs_fstat(char *arg) {
  char com[50] = "mynfs_fstat ";
  strcat(com, arg);
  write(sock, com, 1024);
  printf("mynfs_fstat issued\n");
  return 1;
}

int mynfs_opendir(char *arg) {
	int dd, success = 1;
  char com[50] = "mynfs_opendir ";
  strcat(com, arg);
  
  if(write(sock, com, 1024) == -1) {
  	//mynfs_error
  }
  
  if(read(sock, &dd, sizeof(int)) == -1) {
  	//mynfs_error = 2;
  }
  
  printf("dd %d\n", dd);
  
  if(dd != -1) {
  	write(sock, &success, sizeof(int));
  	add_opened_dir(dd, arg);
  }
  
  return dd;
}

int mynfs_closedir(char *arg) {
	int response, dd,result = 0;
  char com[50] = "mynfs_closedir ";
  strcat(com, arg);
  
  if(write(sock, com, 1024) == -1) {
  	//mynfs_error
  }
  
  if(read(sock, &response, sizeof(int)) == -1) {
  	//mynfs_error = 4;
  }
  
  printf("response = %d\n", response);
  
  if(response == 0) {
  	result = -1;
  	//mynfs_error = 5;
  }
  
  dd = atoi(arg);
  remove_opened_dir(dd);
  printf("mynfs_closedir issued\n");
  return result;
}

int mynfs_readdir(char *arg) {
  char com[50] = "mynfs_readdir ";
  strcat(com, arg);
  write(sock, com, 1024);
  printf("mynfs_readdir issued\n");
  return 1;
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
      res = mynfs_readdir(arg);
    if(!strcmp(com, "mynfs_closedir"))
      res = mynfs_closedir(arg);
    if(!strcmp(com, "mynfs_fstat"))
      res = mynfs_fstat(arg);
    if(!strcmp(com, "list_all"))
      list_all();
    if(!strcmp(com, "close"))
      break;
  }
  close(sock);
}
