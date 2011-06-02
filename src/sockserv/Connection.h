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

#define __CONNECTION_H
#include <pthread.h>
#include <string>
#include <stdlib.h>
#ifndef __SOCKSERV_H
#include "SockServ.h"
#endif
class ConnectionWrapper;
#include <netinet/in.h>
using namespace std;
class Connection {
 public:
  Connection();
  Connection(int sockd, SockServ* serv, struct sockaddr* client);
  int Start();
  void sendMsg(const char* mesg);
  void RunLoop();
  void setRecv(void (*)(const char*,Connection*));
  void onRecv(const char*);
  int conn_desc;
  SockServ* parent;
  ConnectionWrapper* cw;
	string host;
 protected:
  static void *DoLoop(void *);
 private: 
  static void display(const char*);
  pthread_t thrd;
  bool hasRecvCallback;
  void (*recvCallback)(const char*,Connection*);
	sockaddr* client;
};
