# This file is part of Derpnet.
# 
# Derpnet is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Derpnet is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Derpnet.  If not, see <http://www.gnu.org/licenses/>.
#
# Copyright (C) 2011 The Derpnet Team.

FLAGS = -pthread -Isockserv -Imulticast -I.
SUBDIRS = multicast sockserv ircserv
.PHONY : subdirs $(SUBDIRS)
.DEFAULT_GOAL := derpnet
subdirs: $(SUBDIRS)
$(SUBDIRS) :
	$(MAKE) -C $@

main.o : main.cpp 
	g++ -c $(FLAGS) main.cpp

derpnet : $(SUBDIRS) main.o config
	g++ -o derpnet $(FLAGS) main.o sockserv/*.o multicast/*.o ircserv/*.o

clean :
	find . -type f -name "*.o" -delete
	rm -f derpnet

config :
	./checkConfigFile.sh

all : clean derpnet

