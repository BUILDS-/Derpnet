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

#ifndef __USER_H
#define __USER_H

#include <string>
using namespace std;

class Channel;
class IrcConn;

class User { 
	public :
		User();
		User(IrcConn*);
		string hostname;
		bool local;
		IrcConn* connection;
		string nick, awayMsg;
};

#endif	
