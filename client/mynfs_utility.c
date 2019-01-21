#include "mynfs_utility.h"

/*
 * function: init_client_socket
 *
 * initializes global socket sock
 *
 * host - IP address of the server
 * port - port to connect to the server on
 */
void init_client_socket(char *host, char *port) {
  struct sockaddr_in server;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1) {
    perror("opening stream socket");
    exit(1);
  }
  
  server.sin_addr.s_addr = inet_addr(host);
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(port));

  if(connect(sock, (struct sockaddr *) &server, sizeof server) == -1) {
    perror("connecting stream socket");
    exit(1);
  }

  printf("Type \"help\" to list available commands\n");
}

/*
 * function: add_opened_file
 *
 * adds new opened file on the server to opened_files_arr
 *
 * fd - server file descriptor
 * path - server file path
 * flags - flags the file was opened with
 */
void add_opened_file(int fd, char *path, int flags) {
	printf("add_opened_file\n");
  opened_files_arr.opened_files[opened_files_arr.num_opened_files].file_descriptor = fd;
  strcpy(opened_files_arr.opened_files[opened_files_arr.num_opened_files].filepath, path);
  opened_files_arr.opened_files[opened_files_arr.num_opened_files].flags = flags;
  opened_files_arr.num_opened_files++;
}

/*
 * function: remove_opened_file
 *
 * removes record specified by fd from opened_files_arr
 *
 * fd - server file descriptor
 */
void remove_opened_file(int fd) {
  int i;

  for(i = 0; i < opened_files_arr.num_opened_files; i++) {
    if(opened_files_arr.opened_files[i].file_descriptor == fd) {
      opened_files_arr.opened_files[i].file_descriptor = 0;
      strcpy(opened_files_arr.opened_files[i].filepath, "");
      opened_files_arr.opened_files[i].flags = -1;
      
      for(int j = i; j < opened_files_arr.num_opened_files-1; j++) {
        opened_files_arr.opened_files[j].file_descriptor =
          opened_files_arr.opened_files[j+1].file_descriptor;
				strcpy(opened_files_arr.opened_files[j].filepath,
          opened_files_arr.opened_files[j+1].filepath);
				opened_files_arr.opened_files[j].flags = opened_files_arr.opened_files[j+1].flags;
      }

      opened_files_arr.num_opened_files--;
      break;
    }
  }
}

/*
 * function: add_opened_dir
 *
 * adds new opened dir on the server to the opened_dirs_arr
 *
 * dd - server directory descriptor
 * path - path to the directory on the server
 */
void add_opened_dir(int dd, char *path) {
  opened_dirs_arr.opened_dirs[opened_dirs_arr.num_opened_dirs].dir_descriptor = dd;
  strcpy(opened_dirs_arr.opened_dirs[opened_dirs_arr.num_opened_dirs].dirpath, path);
  opened_dirs_arr.num_opened_dirs++;
}

/*
 * function: remove_opened_dir
 *
 * removes record specified by dd from the opened_dir_arr
 *
 * dd - server directory descriptor
 */
void remove_opened_dir(int dd) {
  int i;

  for(i = 0; i < opened_dirs_arr.num_opened_dirs; i++) {
    if(opened_dirs_arr.opened_dirs[i].dir_descriptor == dd) {
      opened_dirs_arr.opened_dirs[i].dir_descriptor = 0;
      strcpy(opened_dirs_arr.opened_dirs[i].dirpath, "");

      for(int j = i; j < opened_dirs_arr.num_opened_dirs-1; j++) {
        opened_dirs_arr.opened_dirs[j].dir_descriptor =
          opened_dirs_arr.opened_dirs[j+1].dir_descriptor;
				strcpy(opened_dirs_arr.opened_dirs[j].dirpath,
          opened_dirs_arr.opened_dirs[j+1].dirpath);
      }

      opened_dirs_arr.num_opened_dirs--;
      break;
    }
  }
}

/*
 * function: list_all
 *
 * lists all opened files and directories on the server
 */
void list_all() {
  printf("\nOpened files: \n");
  if(opened_files_arr.num_opened_files > 0) {
    for(int i = 0; i < opened_files_arr.num_opened_files; i++) {
      printf("%d  ", opened_files_arr.opened_files[i].file_descriptor);
      printf("%s  ", opened_files_arr.opened_files[i].filepath);
      printf("%d\n", opened_files_arr.opened_files[i].flags);
    }
  } else {
    printf("none\n");
  }
  
  printf("Opened dirs: \n");
  if(opened_dirs_arr.num_opened_dirs > 0) {
    for(int i = 0; i < opened_dirs_arr.num_opened_dirs; i++) {
      printf("%d  ", opened_dirs_arr.opened_dirs[i].dir_descriptor);
      printf("%s\n", opened_dirs_arr.opened_dirs[i].dirpath);
    }
  } else {
    printf("none\n");
  }
}


void send_success() {
  int success = 1;

  if(write(sock, &success, sizeof(int)) == -1) {
    perror("send_success failed");
  }
}

void send_failure() {
  int failure = 0;

  if(write(sock, &failure, sizeof(int)) == -1) {
    perror("send_failure failed");
  }
}
