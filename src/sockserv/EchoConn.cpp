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

EchoConn::EchoConn(int sockd) {
  this->conn_desc = sockd;
}

void EchoConn::RunLoop() {
  char buf[4];
  bzero(buf,4);
  int stat;
  string msgbuf(""), line("");
  int llength;
  while(true) {
    stat = read(conn_desc,buf,3);
    if(stat == -1) {
	  pthread_exit(NULL);
      return;
    } else if (stat == 0) {
      pthread_exit(NULL);
      return;
    } else {
      msgbuf += string(buf).substr(0,stat);
      llength = msgbuf.find('\n');
      if(llength != -1) {
        line = msgbuf.substr(0,llength+1);
        msgbuf = msgbuf.substr(llength+1);
  	    write(conn_desc,line.c_str(),llength);
        if(line.compare("EXIT\n") == 0) {
		  close(conn_desc);
          pthread_exit(NULL);
		  return;
		}
	  }
    }
  }
}
int EchoConn::Start() {
  return pthread_create(&thrd, NULL, EchoConn::DoLoop, (void *) conn_desc);
}

void * EchoConn::DoLoop(void* args) {
  int c;
  c = (int)args;
  EchoConn(c).RunLoop();
}
