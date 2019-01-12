#include "server.h"

int main(int argc, char **argv) {
  init_server_socket();
  server_exec();
  return 0;
}
