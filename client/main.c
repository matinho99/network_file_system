#include "client.h"

int main(int argc, char *argv[])
{
  if(argc != 3) {
    printf("Invalid number of arguments\n");
    exit(-1);
  }

  init_client_socket(argv[1], argv[2]);
  client_exec();
  exit(0);
}
