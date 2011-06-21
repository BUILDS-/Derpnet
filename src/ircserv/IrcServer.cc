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
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "irc.h"
#include "User.h"

#include "IrcServer.h"


#define hash_map unordered_map
#define get_memfun(object,fnPtr)	((object).*(fnPtr)) //Sorry for #define macros but in this case it's a marginally better idea than the awful normal syntax
using namespace std;

IrcServer::IrcServer() {
  this->conns = new list<IrcConn*>();
	this->users = new list<User*>();
	this->chans = new list<Channel*>();
	this->lineHandlers = new hash_map<string,lineHandler>();
	this->hostname = "unknown";
	this->servername = "unknown";
	this->motd_file = "";
	addFunctionPointers();
	readConfig("irc.conf");
}

void IrcServer::addFunctionPointers() {
	(*lineHandlers)["nick"]=&IrcServer::nickLine;
	(*lineHandlers)["user"]=&IrcServer::userLine;
	(*lineHandlers)["privmsg"]=&IrcServer::privmsgLine;
	(*lineHandlers)["mode"]=&IrcServer::modeLine;
	(*lineHandlers)["ping"]=&IrcServer::pingLine;
	(*lineHandlers)["quit"]=&IrcServer::quitLine;
}

bool IrcServer::readConfig(string filename) {
	ifstream configFile(filename.c_str());
	string currentLine;
	int lineNo = 0;
	if(configFile.is_open()) {
		while(configFile.good()) {
			getline(configFile,currentLine);
			handleConfig(currentLine, lineNo++);
		}
	configFile.close();
	} else {
		printf("Error: Could not open configuration file %s\n", filename.c_str());
		return false;
	}
	fullhost = servername + "." + hostname;
	return true;
}

void IrcServer::handleConfig(string line, int lineNo) {
	string key, setting;
	if(line.size() == 0) {
		return; //empty line
	}
	if(line.at(0) == '#') {
		return; //Line is a comment
	}
	int equalsIndex = line.find('=');
	if(equalsIndex == -1) { 
		if(line.find_first_not_of(WHITESPACE) != -1) {
			printf("Error in line %d of config: Expected '=' but none found. Proceeding anyway.\n",lineNo);
		}
	} else {
		key = toLower(trim(line.substr(0,equalsIndex)));
		setting = toLower(trim(line.substr(equalsIndex+1)));
		//printf("Key %s = setting %s \n", key.c_str(), setting.c_str());
		if(key.compare("hostname") == 0) 
			this->hostname = setting;
		if(key.compare("server") == 0)
			this->servername = setting;
		if(key.compare("motd") == 0)
			this->motd_file = setting;
		
	}
}


void IrcServer::addConnection(Connection connection) {
  IrcConn* mc = new IrcConn(this, connection);
  this->conns->push_back(mc);
  printf("Connection added\n");
}

void IrcServer::handleLine(IrcConn* c, bool hasPrefix, string prefix, string command, vector<string>* params) {

  //Convert the command to lowercase for sanity.

	command = toLower(command);

	if(lineHandlers->count(command.c_str()) == 1) { 
		get_memfun(*this,(*lineHandlers)[command.c_str()])(c,params); //I totally don't trust that this method isn't going to segfault.
	} else {
		//Unrecognized command!
		printf("Can't interpret line with command %s\n",command.c_str());
	}
}
	

void IrcServer::nickLine(IrcConn* c, vector<string>* params) {
	//NICK line. Params = <nick>
  if(params->size() < 1) {
	  noNickGiven(c); //ERR_NONICKNAMEGIVEN
  } else {
		string nick = params->at(0);
		if(validNickname(nick)) {
			c->nick = nick;
			c->nick_lower = toLower(nick);
			c->isNicked = true;
			if(c->isUsered) {
				initial(c);
				c->isReady = true;
			}
		} else {
			errNick(c,nick); //ERR_ERRONEOUSNICKNAME
		}
  }
}

void IrcServer::userLine(IrcConn* c, vector<string>* params) {
	//USER line. Params = <user> <mode> <unused> <realname>
  if(params->size() < 4) { 
	  //ERR_NEEDMOREPARAMS
  } else {
		string uname = params->at(0);
		c->user = uname;
		c->isUsered = true;
	  if(c->isNicked) {
			initial(c);
			c->isReady = true;
		}
	}
}

void IrcServer::privmsgLine(IrcConn* c, vector<string>* params) {
	//PRIVMSG line. Params = <target> <message>
	if(params->size() < 2) { 
		//ERR_RECIPIENT
	} else if(params->size() < 2) {
		//ERR_NOTEXTTOSEND
	} else {
		string targets = params->at(0);
		string message = params->at(1);
		if(validNickname(targets)) {
			IrcConn* target = getConnectionByNick(targets);
			if(target == NULL) {
				//ERR_NOSUCHNICK
			} else {
				target->sendCommand(c->getTitle(), "PRIVMSG", target->nick + " :" + message);
			}
		}
	}
}

void IrcServer::modeLine(IrcConn* c, vector<string>* params) {
	if(params->size() < 1) {
		//ERR_NEEDMOREPARAMS
	} else {
		if(toLower(params->at(0)).compare(c->nick_lower) != 0) {
			//ERR_USERSDONTMATCH
		} else {
			if(params->size() < 2 ) {
				//RPL_UMODEIS
			} else {
				//Set modes.
			}
		}
	}
}

void IrcServer::pingLine(IrcConn* c, vector<string>* params) {
	if(params->size() < 1) {
		//ERR_NOORIGIN
	} else if(params->size() == 1) {
		if(toLower(params->at(0)).compare(toLower(fullhost)) == 0) {
			c->sendCommand(fullhost, "PONG", ":" + fullhost);
		}
	} else {
	}
}

void IrcServer::quitLine(IrcConn* c, vector<string>* params) {
	if(params->size() == 1) {
		//QUIT them with a message
	} else {
		//QUIT them without one
	}
}


void IrcServer::initial(IrcConn* c) {
	welcome(c); //RPL_WELCOME
	yourHost(c); //RPL_YOURHOST
	created(c); //RPL_CREATED
	myInfo(c); //RPL_MYINFO
	iSupport(c); //RPL_ISUPPORT
	luserClient(c); //RPL_LUSERCLIENT
	luserOp(c); //RPL_LUSEROP
	luserChan(c); //RPL_LUSERCHANNELS
	luserMe(c); //RPL_LUSERME
	motd(c);
	addUserFromConn(c);
}

void IrcServer::motd(IrcConn* c) {
	ifstream motdStream(motd_file, ifstream::in);
	if(!motdStream) {
		noMotd(c); //ERR_NOMOTD
		return;
	}
	motdStart(c);
	char line[80];
	while(motdStream.good()) {
		motdStream.getline(line,80);
		motdBody(c,string(line));
	}
	motdStream.close();
	motdEnd(c);
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

void IrcServer::addUserFromConn(IrcConn* c) {
	User* u = new User(c);
	c->setUser(u);
	users->push_back(u);
}

void IrcServer::numericLine(IrcConn* c, int code, string message) {
	char codeS[4];
	sprintf(codeS,"%03d",code%1000);
	c->sendCommand(fullhost,codeS,c->nick + " :" + message);
}

void IrcServer::numericLiteral(IrcConn* c, int code, string message) {
	char codeS[4];
	sprintf(codeS,"%03d",code%1000);
	c->sendCommand(fullhost,codeS,c->nick + " " + message);
}

void IrcServer::welcome(IrcConn* c) {
	//RPL_WELCOME 
	numericLine(c,RPL_WELCOME,"Welcome to the Derpnet IRC Network " + c->nick + "!" + c->user + "@" + c->host);
}

void IrcServer::yourHost(IrcConn* c) {
	//RPL_YOURHOST
	numericLine(c,RPL_YOURHOST,"Your host is " + servername + ", running version " + IRCD_VERSION);
}

void IrcServer::created(IrcConn* c) {
	//RPL_CREATED
	numericLine(c,RPL_CREATED,"This server was created at Three in the Morning (RJ'S I Can Barely Sleep In This Casino Remix), June 1st, 2011");
}

void IrcServer::myInfo(IrcConn* c) {
	//RPL_MYINFO
	numericLine(c,RPL_MYINFO,fullhost + " " + IRCD_VERSION + " i r");
}

void IrcServer::iSupport(IrcConn* c) {
	//RPL_ISUPPORT. See the ISUPPORT document.
	string supports("");
	supports += "PREFIX=(hov)%@+ ";
	supports += "NICKLEN=30 ";
	supports += "CHANTYPES=# ";
	supports += "TOPICLEN=300 ";
	supports += "CHANLIMIT=30 ";

	supports += "are supported by this server";
	numericLine(c,RPL_ISUPPORT,supports);
}

void IrcServer::luserClient(IrcConn* c) {
	//RPL_LUSERCLIENT
	numericLine(c,RPL_LUSERCLIENT,"There are " + toString(users->size()) + " users and " + toString(0) + " services on " + toString(1) + " servers");
}

void IrcServer::luserOp(IrcConn* c) {
	//RPL_LUSEROP
	numericLiteral(c,RPL_LUSEROP,toString(0) + " :operator(s) online");
}

void IrcServer::luserChan(IrcConn* c) {
	//RPL_LUSERCHANNELS
	numericLiteral(c,RPL_LUSERCHANNELS,toString(chans->size()) + " :channels formed");
}

void IrcServer::luserMe(IrcConn* c) { 
	//RPL_LUSERME
	numericLine(c,RPL_LUSERME,"I have " + toString(conns->size()) + " clients and " + toString(0) + " servers");
}

void IrcServer::away(IrcConn* c, User* attempted) { 
	//RPL_AWAY
	numericLiteral(c,RPL_AWAY,attempted->nick + " :" + attempted->awayMsg);
}

void IrcServer::notAway(IrcConn* c) {
	//RPL_UNAWAY
	numericLine(c,RPL_UNAWAY,"You are no longer marked as being away");
}

void IrcServer::nowAway(IrcConn* c) {
	//RPL_NOWAWAY
	numericLine(c,RPL_NOWAWAY,"You have been marked as being away");
}

void IrcServer::motdBody(IrcConn* c, string line) {
	//RPL_MOTD
	numericLine(c,RPL_MOTD,"- " + line.substr(0,80)); //RFC mandates length is no longer than 80 characters (unlike this comment :33)
}

void IrcServer::motdStart(IrcConn* c) {
	//RPL_MOTDSTART
	numericLine(c,RPL_MOTDSTART,"- " + fullhost + " Message of the day - ");
}

void IrcServer::motdEnd(IrcConn* c) {
	//RPL_ENDOFMOTD
	numericLine(c,RPL_ENDOFMOTD,"End of MOTD command");
}

void IrcServer::noNickGiven(IrcConn* c) {
	//ERR_NONICKNAMEGIVEN
	numericLine(c,ERR_NONICKNAMEGIVEN,"No nickname given");
}

void IrcServer::noMotd(IrcConn* c) {
	//ERR_NOMOTD
	numericLine(c,ERR_NOMOTD,"MOTD file is missing");
}

void IrcServer::errNick(IrcConn* c,string nick) {
	//ERR_ERRONEOUSNICKNAME
	numericLiteral(c,ERR_ERRONEOUSNICKNAME,nick + " :Erroneous nickname");
}

void IrcServer::nickUsed(IrcConn* c, string nick) {
	//ERR_NICKNAMEINUSE
	numericLiteral(c,ERR_NICKNAMEINUSE,nick + " :Nickname is already in use.");
}

//END OF IRCSERVER METHODS

string toLower(string input) {
	string s = input.substr();
	transform(s.begin(), s.end(), s.begin(), (int(*)(int)) tolower);
	return s;
}

string toString(int num) {
	if(num == 0 ) {
		return "0";
	}
	if(num < 0) { 
		return "-" + toString(-num);
	}
  char numS[2+(int)(log(num)/log(10))];
	sprintf(numS,"%d",num);
	return string(numS);
}

string trim(string input) {
	int firstNonSpace = input.find_first_not_of(WHITESPACE);
	int lastNonSpace = input.find_last_not_of(WHITESPACE);
	if(lastNonSpace == -1) {
		return string("");
	}
	return input.substr(firstNonSpace, lastNonSpace-firstNonSpace+1);
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


