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

/* 
* Connection is a simple socket wrapper - it will deal with socket stuff
* Will allow defind hooks for receiving messages + disconnecting + etc
* Also .send and .close
*/

#include "Connection.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <ConnectionWrapper.h>
using namespace std;
struct conn_info {
  int sockd;
  SockServ* serv;
  sockaddr* client;
};
Connection::Connection() {
  //i dunno wut
}

Connection::Connection(int sockd, SockServ* serv, struct sockaddr *client) {
  this->conn_desc = sockd;
  this->parent = serv;
  this->hasRecvCallback = false;
}

void Connection::RunLoop() {
  char buf[64];
  bzero(buf,64);
  int stat;
  string msgbuf(""), line("");
  int llength;
  while(true) {
    //read bytes from socket. If the message so far has a newline, send it up to the newline.
    stat = read(this->conn_desc,buf,63);
    if(stat == -1) {
      try {
      } catch(...) {
      }
      printf("Exploding\n");
      parent->connections.remove(conn_desc);
      pthread_exit(NULL);
      return;
    } else if (stat == 0) {
      try {
      } catch(...) {
      }
      printf("Dying\n");
      parent->connections.remove(conn_desc);
      pthread_exit(NULL);
      return;
    } else {
      msgbuf += string(buf).substr(0,stat);
      llength = msgbuf.find('\n');
      while(llength != -1) {
        line = msgbuf.substr(0,llength+1);
        msgbuf = msgbuf.substr(llength+1);
        try {
          onRecv(line.substr(0,llength).c_str());
        } catch(...) {
          printf("ERROR!\n");
        }
        if(line.compare("EXIT\n") == 0) {
          printf("Client connection %d requested close. Complying.\n", conn_desc);
          close(conn_desc);
          parent->connections.remove(conn_desc);
          pthread_exit(NULL);
          return;
        }
        llength = msgbuf.find('\n');
      }
    }
  }
}

int Connection::Start() {
  //threading ;_;
  int x = pthread_create(&thrd, NULL, Connection::DoLoop, (void *) this);
  return x;
}

void Connection::sendMsg(const char* msg) {
  char newMsg[strlen(msg)+1];
  int length = sprintf(newMsg,"%s\n",msg);
  send(conn_desc,newMsg,length,MSG_NOSIGNAL);
}

void Connection::display(const char* mesg) {
    printf("%s\n",mesg);
}

//Static method for starting the actual loop after the thread has been split
void * Connection::DoLoop(void* args) {
  Connection* c = (Connection*)args;
  try {
    c->RunLoop();
  } catch (int e) {
    printf("ERROR\n");
    printf("ERROR: %d\n",e);
  }
}

void Connection::setRecv(void (* cb)(const char*,Connection*)) {
  hasRecvCallback = true;
  recvCallback = cb;
}

void Connection::onRecv(const char * input) {
  cw->onReceive(input);
}
/* vim: set expandtab sw=2: */
