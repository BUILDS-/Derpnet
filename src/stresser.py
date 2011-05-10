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
