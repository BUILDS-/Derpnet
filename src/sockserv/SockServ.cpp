#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "SockServ.h"
#include "EchoConn.cpp"

SockServ::SockServ() {
  //TODO
  MAX_CONNECTIONS = 10;
  MAX_INC_CONNECTIONS = 5;
}

bool SockServ::beginListen(int port, int conn_type) {
  //Build the socket address (AF_INET,"",<port>)
  struct sockaddr_in dest, client;
  dest.sin_family = AF_INET;
  dest.sin_port = htons(port);
  dest.sin_addr.s_addr = htonl (INADDR_ANY);

  printf("Port: %d; Type: %d\n",port,conn_type);
  printf("Getting socket\n");
  listener = socket(AF_INET,SOCK_STREAM,0);
  if(listener == -1) {
    printf("Socket acquire failed.\n");
  } else {
    printf("Succeded!\n");
  }
  printf("Descriptor: %d\n",listener);
  printf("Binding socket\n");
  if(bind(listener, (struct sockaddr *)&dest, sizeof dest) == -1) {
    printf("Bind failed D:\n");
    perror("derp!!!\n");
    return false;
  } else {
    printf("Bind succeded :D\n");
  }
  printf("Listening\n");
  if(listen(listener,MAX_INC_CONNECTIONS) == -1) {
    printf("Listen failed.\n");
	return false;
  } else {
    printf("Listen succeeded!\n");
  }
  while(true) {
    socklen_t addr_sz = sizeof client;
    int client_desc = accept(listener,(struct sockaddr *)&client,&addr_sz);
    if(client_desc == -1) {
      printf("Accept failed!\n");
      return false;
    } else {
      printf("Client descriptor: %d\n", client_desc);
    }
    EchoConn newConn(client_desc);
    newConn.Start();
  }
  return true;
}


