/*
* This file is part of Derpnet.
*
* Derpnet is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Derpnet is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Derpnet.  If not, see <http://www.gnu.org/licenses/>.
*
* Copyright (C) 2011 The Derpnet Team.
*/

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include "sockserv/SockServ.h"
#include "sockserv/Connection.h"
#include "multicast/MultiServer.h"

MultiServer* m = new MultiServer();
void addConnection(int desc, SockServ* parent, struct sockaddr* client) {
  Connection newConn = Connection(desc, parent,client);
  m->addConnection(newConn);
}
//Basic echoserver code. Running on 24.63.226.212:6667 right now.
int main(int argc, char *argv[]) {
  SockServ srv = SockServ();
  srv.setCallBack((&addConnection));
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
