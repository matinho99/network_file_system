#include "client.h"

int main(int argc, char *argv[])
{
  int sock;
  struct sockaddr_in server;
  struct hostent *hp, *gethostbyname();
  char buf[1024];
  
  char* hname = argv[1];

  /* Create socket */
  /*sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1) {
    perror("opening stream socket");
    exit(1);
  }

  /* Get the IP address from hostname */
  server.sin_family = AF_INET;
  hp = gethostbyname(hname);

  if(hp == (struct hostent *) 0) {
    fprintf(stderr, "%s: unknown host\n", hname);
    exit(2);
  }

  memcpy((char *) &server.sin_addr, (char *) hp->h_addr, hp->h_length);
  server.sin_port = htons(atoi( argv[2]));

  if(connect(sock, (struct sockaddr *) &server, sizeof server) == -1) {
    perror("connecting stream socket");
    exit(1);
  }

  printf("Type \"help\" to list available commands\n");
  
  /* main loop */
  while(1) {
	int n = 64, c;
	char *str, getstr[n], *com, *arg;
    str = fgets(getstr, n, stdin);
	// if(str == NULL)
	
	str = strtok(str, "\n");
	com = strtok(str, " ");
	if(com == NULL)
	  com = str;
    else
	  arg = strtok(NULL, " ");
	
	int res;
	
    if(!strcmp(com, "help"))
	  help();
    if(!strcmp(com, "mynfs_open"))
	  res = mynfs_open(arg);
    if(!strcmp(com, "mynfs_read"))
	  res = mynfs_read();
    if(!strcmp(com, "mynfs_write"))
	  res = mynfs_write(arg);
    if(!strcmp(com, "mynfs_lseek"))
	  res = mynfs_lseek(arg);
    if(!strcmp(com, "mynfs_close"))
	  res = mynfs_close();
    if(!strcmp(com, "mynfs_unlink"))
	  res = mynfs_unlink(arg);
    if(!strcmp(com, "mynfs_opendir"))
	  res = mynfs_opendir(arg);
    if(!strcmp(com, "mynfs_readdir"))
	  res = mynfs_readdir();
    if(!strcmp(com, "mynfs_closedir"))
	  res = mynfs_closedir();
    if(!strcmp(com, "mynfs_fstat"))
	  res = mynfs_fstat();
    if(!strcmp(com, "close"))
	  break;
  }
  
  close(sock);
  exit(0);
}
