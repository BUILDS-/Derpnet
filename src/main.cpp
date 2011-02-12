#include <iostream>
#include <stdio.h>
#include "sockserv/SockServ.cpp"

int main(int argc, char *argv[]) {
 SockServ srv;
 srv = SockServ();
 if(srv.beginListen(6667,CONN_CLIENT)) {
  printf("all derps successfully herped.\n");
 } else {
  perror("error code 0x92FE00: missing finland.hpp\n");
 }
 
}
