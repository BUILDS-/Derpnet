import socket
import sys
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((sys.argv[1], int(sys.argv[2])))
connClosed = False
while not connClosed :
    strng = raw_input('>')
    if len(strng) == 0 :
        print "Please enter a legit message"
    else :
        s.send(strng + '\n')
        rcv = s.recv(len(strng) + 20)
        if len(rcv) == 0 :
            print "Remote host closed connection."
            connClosed = True
        else :
            print rcv
