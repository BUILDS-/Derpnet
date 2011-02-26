import threading
import socket
import curses
import time
def initSc() :
  screen = curses.initscr()
  curses.noecho()
  curses.cbreak()
  screen.keypad(1)
  curses.curs_set(0)
  return screen

def clSc(screen) :
  curses.nocbreak()
  screen.keypad(0)
  curses.echo()
  curses.endwin()
def runInput(screen, window, sock) :
  curses.nonl()
  try :
    x = 1
    llength = 0
    s = ""
    while x == -1 or not chr(x) == "q"  :
      x = window.getch()
      if not x == -1:
        if x == 13 :
          sock.send(s + '\n')
          s = ""
          llength = 0
        else :
          s += chr(x)
          window.addstr(1,llength, chr(x))  
          llength += 1
        if x == ord('q') :
          return
      window.refresh()
  except :
    return

def runOut(screen, window, sock) :
  mesgs = []
  buf = ""
  while True:
    s = buf + sock.recv(32)
    mesgs.extend(s.split("\n")[:-1])
    buf = s.split("\n")[-1]
    y = 0
    for i in mesgs[-10:] : 
      window.addstr(y,0,i)
      y+=1
    window.refresh()
screen = initSc()
(HEIGHT,WIDTH) = screen.getmaxyx()
linesWin = curses.newwin(HEIGHT-3,WIDTH,0,0)
inputWin = curses.newwin(3,WIDTH,HEIGHT-3,0)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('127.0.0.1',6668))
try :
  t = threading.Thread(target=runInput,args=(screen,inputWin,s))
  t.start()
  runOut(screen,linesWin,s)
except :
  pass
s.close()
clSc(screen)
