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

#include "EchoConn.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

using namespace std;

EchoConn::EchoConn() {
  //i dunno wut
}

EchoConn::EchoConn(int sockd, SockServ* serv, struct sockaddr *client) {
  this->conn_desc = sockd;
  this->parent = serv;
}

void EchoConn::RunLoop() {
  char buf[64];
  bzero(buf,64);
  int stat;
  string msgbuf(""), line("");
  int llength;

  while(true) {
    //read bytes from socket. If the message so far has a newline, send it up to the newline.
    stat = read(conn_desc,buf,63);
    if(stat == -1) {
      printf("Client connection %d exploded socket. Closing.\n",conn_desc);
      parent->connections.remove(conn_desc);
	  pthread_exit(NULL);
      return;
    } else if (stat == 0) {
      printf("Client connection %d closed socket. Closing.\n",conn_desc);
      parent->connections.remove(conn_desc);
      pthread_exit(NULL);
      return;
    } else {
      msgbuf += string(buf).substr(0,stat);
      llength = msgbuf.find('\n');
      if(llength != -1) {
        line = msgbuf.substr(0,llength+1);
        msgbuf = msgbuf.substr(llength+1);
        try {
          parent->sendMessage(line);
        } catch(...) {
          printf("Send error\n");
        }
        if(line.compare("EXIT\n") == 0) {
		  printf("Client connection %d requested close. Complying.\n", conn_desc);
		  close(conn_desc);
      parent->connections.remove(conn_desc);
          pthread_exit(NULL);
		  return;
		}
	  }
    }
  }
}

int EchoConn::Start() {
  //threading ;_;
  return pthread_create(&thrd, NULL, EchoConn::DoLoop, (void *) this);
}

void EchoConn::sendMsg(string msg) {}

//Static method for starting the actual loop after the thread has been split
void * EchoConn::DoLoop(void* args) {
  EchoConn c;
  c = *((EchoConn *)args);
  try {
    EchoConn(c).RunLoop();
  } catch (int e) {
    printf("ERROR\n");
    printf("ERROR: %d\n",e);
  }
}
