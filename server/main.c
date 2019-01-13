#include "server.h"

int main(int argc, char **argv) {
  load_client_accesses();
  server_exec();
  return 0;
}
