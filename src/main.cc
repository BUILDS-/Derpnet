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
#include <string.h>
#include <sys/socket.h>
#include "sockserv/SockServ.h"
#include "sockserv/Connection.h"
#include "multicast/MultiServer.h"
#include "ircserv/IrcServer.h"
#include "errors.h"
#include "globals.h"

IrcServer* server;

void addConnection(int desc, SockServ* parent, struct sockaddr* client) {
  Connection newConn = Connection(desc, parent,client);
  server->addConnection(newConn);
}
//Basic echoserver code. Running on 24.63.226.212:6667 right now.
int main(int argc, char *argv[]) {
  int port = 6667;
  //Get port number from argv[1]
	int i = 1;
	while(i < argc) {
		if(!strcmp(argv[i],"-p")) {
			i++;
			if(i < argc) { 
				port = atoi(argv[i]);
				i++;
			} else {
				printf("-p requires a parameter\n");
			}
		} else if(!strcmp(argv[i],"--help")) {
			printf("Usage: derpnet [-p port] [--colors] [--nocolors]\n");
			i++;
		} else if(!strcmp(argv[i],"--colors")) {
			hasColors = true;
			i++;
		} else if(!strcmp(argv[i],"--nocolors")) {
			hasColors = false;
			i++;
		} else {
			printf("Invalid argument: %s\n",argv[i]);
			i++;
		}
	}
  server = new IrcServer();
  SockServ srv = SockServ();
  srv.setCallBack((&addConnection));
 //Perform listen loop
  statusMsg("Listening on port %d", port);
  if(srv.beginListen(port,CONN_CLIENT)) {
    statusMsg("Successfully listened on port %d\n",port);
    return 0;
  } else {
		majorError("Problem during listening: %s",strerror(errno));
		exit(errno);
    return errno;
  }
 
}
