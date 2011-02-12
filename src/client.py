import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('24.63.226.212',6667))
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
