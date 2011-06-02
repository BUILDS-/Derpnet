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

#include "MultiConn.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef __SERVER_H
#include <server.h>
#endif
#ifndef __MULTISERVER_H
#include "MultiServer.h"
#endif

using namespace std;

MultiConn::MultiConn() {
	//i dunno wut
}

MultiConn::MultiConn(Server* p, Connection c) : ConnectionWrapper(p,c) {
	this->c = c;
	this->c.cw = this;
	this->c.Start();
}

void MultiConn::onReceive(const char* message) {
	((MultiServer*)this->parent)->sendToAll(message);
}

void MultiConn::send(const char* message) {
	c.sendMsg(message);
}
