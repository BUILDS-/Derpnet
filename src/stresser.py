import socket
import time
global tot
global worked
worked = 0
tot = 0

def openConn() :
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('127.0.0.1',6667))
    s.send('test\n' * 5000)
    time.sleep(0.1)
    worked = False
    if len(s.recv(65536)) == 20000 :
        worked = True
    s.close()
    return worked

for i in range(40) :
    if openConn() :
        worked += 1
