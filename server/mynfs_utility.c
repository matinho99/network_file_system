#include "mynfs_utility.h"

void init_server_socket(int *server_sock) {
  struct sockaddr_in addr;

  if((*server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("server_socket creation failed");
    exit(0);
  }

  if (setsockopt(*server_sock, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);

  if(bind(*server_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("server socket bind failed");
    exit(0);
  }
}

void load_client_accesses() {
  int count = 0, len = 30, i;
  char c;
  char *buf = NULL;
  FILE *f = fopen("/home/mosowiec/Workspace/network_file_system/server/accesses", "r");
  
  if(f == NULL) {
    printf("could not open file\n");
  }

  for (c = getc(f); c != EOF; c = getc(f)) 
    if (c == '\n')
      count++;

  printf("count = %d\n", count);
  client_accesses_arr.client_accesses = (struct mynfs_access *)malloc(count*sizeof(struct mynfs_access));
  printf("number of client accesses added: %d\n", (int)(sizeof(client_accesses_arr.client_accesses)/sizeof(struct mynfs_access)));
  fseek(f, 0, SEEK_SET);
  i = 0;

  while(getline(&buf, (size_t *)&len, f) != -1) {
    char *flags;
    printf("retrieved line: %s", buf);
    char *str = strtok(buf, "\n");
    strcpy(client_accesses_arr.client_accesses[i].client_ip, strtok(str, ";"));
    strcpy(client_accesses_arr.client_accesses[i].path, strtok(NULL, ";"));
    flags = strtok(NULL, ";");

    if(!strcmp(flags, "r")) {
      client_accesses_arr.client_accesses[i].flags = O_RDONLY;
    } else if(!strcmp(flags, "w")) {
      client_accesses_arr.client_accesses[i].flags = O_WRONLY;
    } else if(!strcmp(flags, "rw")) {
      client_accesses_arr.client_accesses[i].flags = O_RDWR;
    }

    client_accesses_arr.size++;
    i++;
  }

  for(i=0; i<client_accesses_arr.size; i++) {
    printf("%s %s %d\n", client_accesses_arr.client_accesses[i].client_ip, 
      client_accesses_arr.client_accesses[i].path, client_accesses_arr.client_accesses[i].flags);
  }

  if(buf != NULL) free(buf);
  fclose(f);
}

int has_access_to_dir(struct client_info ci, char *dp) {
  int result = 0;
  int i = 0;

  while(i < client_accesses_arr.size) {
    if(!strcmp(client_accesses_arr.client_accesses[i].client_ip, ci.ip)) {
      char *str = client_accesses_arr.client_accesses[i].path;
      int j = 0;
      result = 1;
      
      while(str[j]!='\0' && dp[j]!='\0') {
        if(dp[j] != str[j]) {
	  result = 0;
          break;
        }

	j++;
      }

      if(result == 1) break;
    }

    i++;
  }

  return result;
}

int has_access_to_file(struct client_info ci, char *fp, int flags) {
  int result = 0;
  int i = 0;

  while(i < client_accesses_arr.size) {
    if(!strcmp(client_accesses_arr.client_accesses[i].client_ip, ci.ip)) {
      char *str = client_accesses_arr.client_accesses[i].path;
      int j = 0;
      result = 1;
      
      while(str[j]!='\0' && fp[j]!='\0') {
        if(fp[j] != str[j]) {
	  result = 0;
          break;
        }

	j++;
      }

      if(result == 1) break;
    }

    i++;
  }

  if(client_accesses_arr.client_accesses[i].flags != O_RDWR && flags != client_accesses_arr.client_accesses[i].flags) {
    result = 0;
  }

  return result;
}

int has_opened_file(struct client_info ci, int fd) {
  int result = 0;
  int i;

  for(i = 0; i < opened_files_arr.num_opened_files; i++) {
    if(opened_files_arr.opened_files[i].file_descriptor == fd 
      && !strcmp(opened_files_arr.opened_files[i].client_ip, ci.ip)) {
      result = 1;
      break;
    }
  }

  return result;
}

int has_opened_file_by_path(struct client_info ci, char *path) {
  int result = 0;
  int i;

  for(i = 0; i < opened_files_arr.num_opened_files; i++) {
    if(!strcmp(opened_files_arr.opened_files[i].filepath, path) 
      && !strcmp(opened_files_arr.opened_files[i].client_ip, ci.ip)) {
      result = 1;
      break;
    }
  }

  return result;
}

int has_opened_dir(struct client_info ci, int dd) {
  int result = 0;
  int i;

  for(i = 0; i < opened_dirs_arr.num_opened_dirs; i++) {
    if(opened_dirs_arr.opened_dirs[i].dir_descriptor == dd 
      && !strcmp(opened_dirs_arr.opened_dirs[i].client_ip, ci.ip)) {
      result = 1;
      break;
    }
  }

  return result;
}

int has_read_access(struct client_info ci, int fd) {
  int result = 0;
  int i = 0;

  while(i < opened_files_arr.num_opened_files) {
    char *client_ip = opened_files_arr.opened_files[i].client_ip;

    if(!strcmp(ci.ip, client_ip)) {
      int flags = opened_files_arr.opened_files[i].flags;

      if(flags == O_RDWR || flags == O_RDONLY) {
	result = 1;
	break;
      }
    }

    i++;
  }

  return result;
}

int has_write_access(struct client_info ci, int fd) {
  int result = 0;
  int i = 0;

  while(i < opened_files_arr.num_opened_files) {
    char *client_ip = opened_files_arr.opened_files[i].client_ip;

    if(!strcmp(ci.ip, client_ip)) {
      int flags = opened_files_arr.opened_files[i].flags;

      if(flags == O_RDWR || flags == O_WRONLY) {
	result = 1;
	break;
      }
    }

    i++;
  }

  return result;
}

int add_opened_file(struct client_info ci, int fd, char *path, int flags) {
  opened_files_arr.opened_files[opened_files_arr.num_opened_files].file_descriptor = fd;
  strcpy(opened_files_arr.opened_files[opened_files_arr.num_opened_files].filepath, path);
  strcpy(opened_files_arr.opened_files[opened_files_arr.num_opened_files].client_ip, ci.ip);
  opened_files_arr.opened_files[opened_files_arr.num_opened_files].flags = flags;
  opened_files_arr.num_opened_files++;
}

int remove_opened_file(struct client_info ci, int fd) {
  int result = 0;
  int i;

  for(i = 0; i < opened_files_arr.num_opened_files; i++) {
    if(opened_files_arr.opened_files[i].file_descriptor == fd 
      && !strcmp(opened_files_arr.opened_files[i].client_ip, ci.ip)) {
      opened_files_arr.opened_files[i].file_descriptor = 0;
      strcpy(opened_files_arr.opened_files[i].client_ip, "");
      opened_files_arr.opened_files[i].flags = -1;
      
      for(int j = i; j < opened_files_arr.num_opened_files-1; j++) {
        opened_files_arr.opened_files[j].file_descriptor =
          opened_files_arr.opened_files[j+1].file_descriptor;
        strcpy(opened_files_arr.opened_files[j].client_ip,
          opened_files_arr.opened_files[j+1].client_ip);
	opened_files_arr.opened_files[j].flags = opened_files_arr.opened_files[j+1].flags;
      }

      opened_files_arr.num_opened_files--;
      result = 1;
      break;
    }
  }

  return result;
}

void send_success(struct client_info ci) {
  char *success = "SUCCESS";

  if(write(ci.sock, success, sizeof success) == -1) {
    perror("send_success failed");
  }
}

void send_failure(struct client_info ci) {
  char *failure = "FAILURE";

  if(write(ci.sock, failure, sizeof failure) == -1) {
    perror("send_failure failed");
  }
}

void send_error(struct client_info ci) {
  char *error = get_error();

  if(write(ci.sock, error, sizeof error) == -1) {
    perror("send_error failed");
  }
}
