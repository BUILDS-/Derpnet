#include <iostream>
#include <stdio.h>
#include "sockserv/SockServ.cpp"

//Basic echoserver code. Running on 24.63.226.212:6667 right now.
int main(int argc, char *argv[]) {
  SockServ srv;
  srv = SockServ();
  int port = 6667;
  //Get port number from argv[1]
  if(argc > 1) {
    port = atoi(argv[1]);
    if(port < 6665 || port > 6675) {
      printf("Port number not recognized or out of range. Stopping.\n");
      return -1;
    }
  }
  //Perform listen loop
  printf("Listening on port %d\n", port);
  if(srv.beginListen(port,CONN_CLIENT)) {
    printf("Successfully listened on port %d\n",port);
    return 1;
  } else {
	printf("Problem!\n");
    perror("Problem during listnening:\n");
    return 0;
  }
 
}
