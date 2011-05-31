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

#define __IRCSERVER_H
#include <pthread.h>
#include <stdlib.h>
#include <queue>
#ifndef __CONNECTION_H
#include <Connection.h>
#endif
#ifndef __IRCCONN_H
#include "IrcConn.h"
#endif
#ifndef __SERVER_H
#include <server.h>
#endif

using std::queue;
class IrcServer : Server{
 public:
  IrcServer();
  void addConnection(Connection);
  void sendToAll(const char*);
  void handleLine(IrcConn*, bool, string, string, vector<string>*);
  bool nickLine(IrcConn*, string);
  bool userLine(IrcConn*, string, string, string);
  void privmsgLine(IrcConn*, vector<string>, string);
  void welcome(IrcConn*);
 private: 
  list<IrcConn*>* conns;
};
