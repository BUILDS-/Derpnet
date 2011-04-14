#define CONN_CLIENT 1
#define CONN_CLIENTOTR 2
#include <list>
#include <string>
using namespace std;
class SockServ
{
 public:
  SockServ();
  //SockServ(Thing daemon);
  bool beginListen(int port, int conn_type);
  void sendMessage(string mesg);
  int MAX_CONNECTIONS, MAX_INC_CONNECTIONS;
  list<int> connections;
 private:
  int listener;
};
