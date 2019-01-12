#ifndef MYNFS_UTILITY_H
#define MYNFS_UTILITY_H

struct mynfs_access {
  char *client_ip;
  char *path;
  int access_level;
};

struct mynfs_opened_file {
  int file_descriptor;
  char *client_ip;
  int file_pointer;
  int access_flag;
};

struct mynfs_opened_dir {
  int dir_descriptor;
  char *client_ip;
  int access_flag;
};

#endif
