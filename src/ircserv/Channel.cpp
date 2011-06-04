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

#include "Channel.h"
#ifndef __IRCCONN_H
#include "IrcConn.h"
#endif
#ifndef __USER_H
#include "User.h"
#endif
#ifndef __IRC_H
#include "irc.h"
#endif
#define hash_map unordered_map

/*
 * For reference: 
 * struct chan_mode { 
 * char name;
 * int type; //One of A_ B_ C_ and D_MODE
 * bool set; //For C and D modes
 * string setting; //For B and C modes
 * list<string>* contents; //For A mode
 * };
*/


Channel::Channel() {
	this->users = new list<User*>();
	this->modes = new hash_map<char,chan_mode*>();
}

Channel::Channel(string name) {
	this->name = name;
	this->users = new list<User*>();
	this->modes = new hash_map<char,chan_mode*>();
}

void Channel::initModes() {
	const char* modesList[] = {MODES_A,MODES_B,MODES_C,MODES_D};
	int i,j;
	for(i=0;i<4;i++) {
		for(j=0;i<sizeof(modesList[i]);i++) { 
			chan_mode* c = new chan_mode();
			c->type = i;
			(*modes)[modesList[j][i]] = c;
		}
	}
}

// vim:ts=4:sw=4
