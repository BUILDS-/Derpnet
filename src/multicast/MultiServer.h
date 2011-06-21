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

#ifndef __MULTISERVER_H
#define __MULTISERVER_H

#include <pthread.h>
#include <stdlib.h>
#include <queue>
#ifndef __CONNECTION_H
#include <sockserv/Connection.h>
#endif
#ifndef __MULTICONN_H
#include "MultiConn.h"
#endif
#ifndef __SERVER_H
#include <server.h>
#endif

using std::queue;
class MultiServer : Server{
 public:
  MultiServer();
  void addConnection(Connection);
  void sendToAll(const char*);
 private: 
  list<MultiConn*>* conns;
};

#endif
