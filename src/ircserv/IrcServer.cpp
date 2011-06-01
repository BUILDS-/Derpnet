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

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "IrcServer.h"
#define ALPHA_L "abcdefghijklmnopqrstuvwxyz"
#define ALPHA_U "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUM "0123456789"
#define SPECIAL "`_+=[]{}|<>"
using namespace std;

IrcServer::IrcServer() {
  this->conns = new list<IrcConn*>();
}

void IrcServer::addConnection(Connection connection) {
  IrcConn* mc = new IrcConn(this, connection);
  mc->host = "unknown-host"; // :33
  this->conns->push_back(mc);
  printf("Connection added\n");
}

void IrcServer::handleLine(IrcConn* c, bool hasPrefix, string prefix, string command, vector<string>* params) {

  //Convert the command to lowercase for sanity.

	command = toLower(command);

  if(command.compare(string("nick")) == 0) { 
	  //RFC: NICK <nick>
	  if(params->size() < 1) {
		  //ERR_NONICKNAMEGIVEN
	  } else {
	   	if(nickLine(c, params->at(0))) {
				c->isNicked = true;
				if(c->isUsered) {
					//send RPL_WELCOME
					welcome(c);
				}
			}
	  }
  }

  if(command.compare(string("user")) == 0) {
	  //RFC: USER <uname> <mode> <unused> <rname>
	  if(params->size() < 4) { 
		  //ERR_NEEDMOREPARAMS
	  } else {
		  if(userLine(c,params->at(0), params->at(1), params->at(3))) {
			  c->isUsered = true;
			  if(c->isNicked) {
				  //send RPL_WELCOME
				  welcome(c);
			  }
		  }
	  }
  } 

	if(command.compare(string("privmsg")) == 0) {
		//RFC: PRIVMSG <targets> <message>
		if(params->size() < 2) {
			//ERR_NOTEXTTOSEND
		} else {
			privmsgLine(c,params->at(0), params->at(1));
		}
	}
}
	

bool IrcServer::nickLine(IrcConn* c, string nick) {
	//NICK line. Params = <nick>
	c->nick = nick;
	c->nick_lower = toLower(nick);
	return true;
}

bool IrcServer::userLine(IrcConn* c, string uname, string mode, string realname) {
	//USER line. Params = <user> <mode> <unused> <realname>
	c->user = uname;
	return true;

}

void IrcServer::privmsgLine(IrcConn* sender, string targets, string message) {
	//PRIVMSG line. Params = <target> <message>
	if(validNickname(targets)) { 
		IrcConn* target = getConnectionByNick(targets);
		if(target == NULL) {
			//ERR_NOSUCHNICK
		} else {
			target->sendCommand(sender->getTitle(), "PRIVMSG", target->nick + " :" + message);
		}
	}
}



void IrcServer::welcome(IrcConn* c) {
	//RPL_Welcome. 
	c->sendCommand("serket.derpnet.oss","001",c->nick + " :Welcome to the Derpnet IRC Network " + c->nick + "!" + c->user + "@" + c->host);
}

bool validNickname(string nickname) {
	if(nickname.size() > 16) { 
		return false;
	}
	if(nickname.find_first_not_of((string)ALPHA_L + ALPHA_U + NUM) == 0) {
		return false;
	}
	if(nickname.find_first_not_of((string)ALPHA_L + ALPHA_U + NUM + SPECIAL) != -1) { 
		return false;
	}
	return true;
}

IrcConn* IrcServer::getConnectionByNick(string nickname) {
	IrcConn* connection = NULL;
	nickname = toLower(nickname);
	for(list<IrcConn*>::iterator it=conns->begin();it!=conns->end();it++) {
		if(((*it)->nick_lower.compare(nickname)) == 0) {
			connection = (*it);
		}
	}
	return connection;
}

string toLower(string input) {
	string s = input.substr();
	transform(s.begin(), s.end(), s.begin(), (int(*)(int)) tolower);
	return s;
}

