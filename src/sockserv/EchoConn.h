#include <pthread.h>
#include <string>
#include <stdlib.h>
using namespace std;
class EchoConn {
 public:
  EchoConn();
  EchoConn(int sockd, SockServ* serv, struct sockaddr* client);
  int Start();
  void sendMsg(string mesg);
  void RunLoop();
 protected:
  static void *DoLoop(void * );
 private: 
  int conn_desc;
  SockServ* parent;
  pthread_t thrd;
};
