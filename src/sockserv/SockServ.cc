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
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <list>
#include <string>
#include <stdio.h>

#include <errors.h>

#include "Connection.h"

#include "SockServ.h"


using namespace std;
SockServ::SockServ() {
	MAX_CONNECTIONS = 10;
	MAX_INC_CONNECTIONS = 5;
}

void SockServ::setCallBack(void (*cb)(int,SockServ*,sockaddr*)) {
	callBack=cb;
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

	partialLine("Getting socket");
	listener = socket(AF_INET,SOCK_STREAM,0);
	if(listener == -1) {
		printf("Failed.\n");
	} else {
		printf("Succeeded.\n");
	}

	partialLine("Binding socket");
	if(bind(listener, (struct sockaddr *)&dest, sizeof dest) == -1) {
		printf("Failed\n");
		return false;
	} else {
		printf("Succeeded\n");
	}

	partialLine("Listening"); //Prepare the socket for getting incoming connections.
	if(listen(listener,MAX_INC_CONNECTIONS*20) == -1) {
		printf("Failed\n");
		return false;
	} else {
		printf("Succeeded\n");
	}

	socklen_t addr_sz = sizeof client;
	int client_desc;
	statusMsg("Now awaiting client connections");
	while(true) { 
		try {
			client_desc = accept(listener,(struct sockaddr *)&client,&addr_sz);
		} catch (int e) {
			errorMsg("Error accepting client connection: %d",e);
		}
		if(client_desc == -1) {
			errorMsg("Client doesn't exist");
		} else {
			statusMsg("Client accepted successfully!");
			connections.push_back(client_desc);
			callBack(client_desc, this, (struct sockaddr* )&client);
		}
	}
	return true;
}

