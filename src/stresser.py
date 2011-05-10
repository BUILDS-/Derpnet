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

import socket
import time
import sys
global tot
global worked
worked = 0
tot = 0

def openConn() :
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((sys.argv[1],int(sys.argv[2])))
    s.send('test\n' * int(sys.argv[3]))
#    time.sleep(2)
    print "Got %d bytes of expected %d\n" % (len(s.recv(65536)), len('test\n' * int(sys.argv[3])))

for i in range(40) :
    openConn()
