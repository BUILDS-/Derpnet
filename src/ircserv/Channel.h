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

#include <queue>
#include <stdlib.h>
#include <unordered_map>
#include <string>
#include <list>
#define __CHAN_H
//Constants for the four types of modes. A brief summary can be found in the
// ISUPPORT reference document.
#define A_MODE 1;
#define B_MODE 2;
#define C_MODE 3;
#define D_MODE 4; 

#define hash_map unordered_map

using namespace std;
class IrcServer;
class IrcConn;
class User;

struct chan_mode { 
	char name;
	int type; //One of A_ B_ C_ and D_MODE
	bool set; //For C and D modes
	string setting; //For B and C modes
	list<string>* contents; //For A mode
};

class Channel {
	public :
		Channel();
		Channel(string);
		string name;
		list<User*>* users;
		hash_map<char,chan_mode*>* modes;
	private :
		void initModes();
};
