#include <pthread.h>

#include <stdlib.h>
class EchoConn {
 public:
  EchoConn();
  EchoConn(int sockd);
  int Start();
  void RunLoop();
 protected:
  static void *DoLoop(void * );
 private: 
  int conn_desc;
  pthread_t thrd;
};
