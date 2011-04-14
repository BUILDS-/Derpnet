#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "SockServ.h"
#include "EchoConn.cpp"
#include <list>
#include <string>
using namespace std;

SockServ::SockServ() {
  //TODO
  MAX_CONNECTIONS = 10;
  MAX_INC_CONNECTIONS = 5;
}
void SockServ::sendMessage(string mesg) {
  list<int>::iterator i;
  for(i = connections.begin(); i != connections.end(); i++) {
    send((*i), mesg.c_str(), mesg.length(), MSG_NOSIGNAL);
  }
}
bool SockServ::beginListen(int port, int conn_type) {
  //Build the socket address (AF_INET,"",<port>)
  struct sockaddr_in dest, client;
  dest.sin_family = AF_INET;
  dest.sin_port = htons(port);
  dest.sin_addr.s_addr = htonl (INADDR_ANY);

  printf("Port: %d; Type: %d\n",port,conn_type);

  printf("Getting socket\n"); //Allocates a socket and binds it to the requested port
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

  printf("Listening\n"); //Prepare the socket for getting incoming connections.
  if(listen(listener,MAX_INC_CONNECTIONS*20) == -1) {
    printf("Listen failed.\n");
	return false;
  } else {
    printf("Listen succeeded!\n");
  }

  socklen_t addr_sz = sizeof client;
  int client_desc;
  while(true) { 
	printf("Waiting to accept an incoming connection...\n");

    try {
      printf("Accepting\n");
      client_desc = accept(listener,(struct sockaddr *)&client,&addr_sz);
      
      printf("Accepted\n");
    } catch (int e) {
      printf("Error Accepting: %d\n",e);
    }
    if(client_desc == -1) {
      printf("Accept failed!\n");
    } else {
      printf("Client descriptor: %d\n", client_desc);
      connections.push_back(client_desc);
      EchoConn newConn(client_desc, this, (struct sockaddr* )&client);
      newConn.Start();
    }
  }
  return true;
}


