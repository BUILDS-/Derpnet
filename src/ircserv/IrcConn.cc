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

#include <server.h>

#include "IrcServer.h"
#include "User.h"

#include "IrcConn.h"


using namespace std;

IrcConn::IrcConn() {
  //i dunno wut
}

IrcConn::IrcConn(IrcServer* p, Connection c) : ConnectionWrapper((Server*)p,c) {
  this->c = c;
  this->c.cw = this;
  this->c.Start();
  this->parent = p;
  this->isServer = false;
  this->isNicked = false;
  this->isUsered = false;
  this->nick = "";
  this->user = "";
  this->host = c.host;
	this->nick_lower = "";
}

void IrcConn::onReceive(const char* message) {
  //The RFC calls for an optional prefix, a command, and then any number of params
  // where the final param can be a trailing param indicated by an initial colon.
  string command, prefix, message_tmp;
  vector<string>* params = new vector<string>();
  string trailing;
  bool hasPrefix = false;
  int nParams = 0;
  message_tmp = string(message);
  //printf("Message: %s\n",message_tmp.c_str());

  //First, look for the prefix which is indicated with an initial colon.
  if(message_tmp.at(0) == ':') {
	  //We have a prefix
	  message_tmp = message_tmp.substr(1);
	  int prefix_index = message_tmp.find(' ');
	  //If that was our entire message, then we'll give an error. Otherwise,
	  // save the prefix and remove it from our message.
	  if(prefix_index != -1) { 
		  prefix = message_tmp.substr(0,prefix_index);
		  message_tmp = message_tmp.substr(
				  message_tmp.find_first_not_of(' ', prefix_index+1));
		  hasPrefix = true;
	  }
  }
  if(hasPrefix) {
	  //printf("Prefix is: %s\n",prefix.c_str());
  }
 
  //The next word should be a command ([a-zA-Z]+) or ([0-9]{3}). We'll check it's
  // a proper command later.
  int command_index = message_tmp.find(' ');
  command = message_tmp.substr(0,command_index);
  message_tmp = message_tmp.substr(
		  message_tmp.find_first_not_of(' ',command_index+1));
  //printf("Command is: %s\n",command.c_str());

  //Now, repeatedly remove words from the string to get the params.
  while(!message_tmp.empty()) {
	  nParams++;
	  if(message_tmp.at(0) == ':') {
		  //This is our trailing option, so we'll take the rest of the line as one
		  // option and exit the loop.
		  trailing = message_tmp.substr(1);
		  params->push_back(trailing);
		  message_tmp.clear();
	  } else {
		  int param_index = message_tmp.find(' ');
		  if(param_index != -1) {
			  params->push_back(message_tmp.substr(0,param_index));
			  int next_index = message_tmp.find_first_not_of(' ',param_index+1);
			  //If there are no more non-space characters, exit.
			  if(next_index != -1) { 
				  message_tmp = message_tmp.substr(next_index);
			  } else{
				  message_tmp.clear();
			  }
		  } else {
		      //If we have no more spaces, then just take the last param.
			  params->push_back(message_tmp.substr());
			  message_tmp.clear();
		  }
	  }
  }
 
  //Let the server execute the command now that we've parsed it.
  parent->handleLine(this, hasPrefix, prefix, command, params);
}

void IrcConn::send(const char* message) {
  c.sendMsg(message);
}

void IrcConn::send(string message) {
  send(message.c_str());
}

void IrcConn::sendCommand(string command, string params) {
  send(command + " " + params);
}

void IrcConn::sendCommand(string prefix, string command, string params) {
  send(":" + prefix + " " + command + " " + params);
}

void IrcConn::quit() {
	this->c.closeConn();
}

string IrcConn::getTitle() { 
	return nick + "!" + user + "@" + host;
}

User* IrcConn::getUser() { 
	return owner;
}

void IrcConn::setUser(User* u) { 
	this->owner = u;
}
