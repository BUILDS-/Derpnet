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

#ifndef __IRCCONN_H
#define __IRCCONN_H

#include <pthread.h>
#include <stdlib.h>
#ifndef __CONNECTIONWRAPPER_H 
#include <ConnectionWrapper.h>
#endif
class Server;
class IrcServer;
class User;
class IrcConn : public ConnectionWrapper{
 public:
  IrcConn();
  IrcConn(IrcServer*, Connection);
  void send(const char*);
  void send(string);
  void sendCommand(string, string);
  void sendCommand(string, string, string);
  void sendCommand(int, string);
  void sendCommand(string, int, string);
	void quit();
	User* getUser();
	void setUser(User*);
  bool isServer;
  bool isNicked, isUsered, isReady;
  string nick, user, host, nick_lower;
	string getTitle();
 private: 
  Connection connection;
  void onReceive(const char*);
  static void onReceiveStatic(const char*, void*);
  IrcServer* parent;
	User* owner;
};

#endif
