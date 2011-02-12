#include "EchoConn.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

using namespace std;

EchoConn::EchoConn() {
  //i dunno wut
}

EchoConn::EchoConn(int sockd) {
  this->conn_desc = sockd;
}

void EchoConn::RunLoop() {
  char buf[4];
  bzero(buf,4);
  int stat;
  string msgbuf(""), line("");
  int llength;
  while(true) {
    stat = read(conn_desc,buf,3);
    if(stat == -1) {
	  pthread_exit(NULL);
      return;
    } else if (stat == 0) {
      pthread_exit(NULL);
      return;
    } else {
      msgbuf += string(buf).substr(0,stat);
      llength = msgbuf.find('\n');
      if(llength != -1) {
        line = msgbuf.substr(0,llength+1);
        msgbuf = msgbuf.substr(llength+1);
  	    write(conn_desc,line.c_str(),llength);
        if(line.compare("EXIT\n") == 0) {
		  close(conn_desc);
          pthread_exit(NULL);
		  return;
		}
	  }
    }
  }
}
int EchoConn::Start() {
  return pthread_create(&thrd, NULL, EchoConn::DoLoop, (void *) conn_desc);
}

void * EchoConn::DoLoop(void* args) {
  int c;
  c = (int)args;
  EchoConn(c).RunLoop();
}
