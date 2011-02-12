#define CONN_CLIENT 1
#define CONN_CLIENTOTR 2

class SockServ
{
 public:
  SockServ();
  //SockServ(Thing daemon);
  bool beginListen(int port, int conn_type);
  int MAX_CONNECTIONS, MAX_INC_CONNECTIONS;
 private:
  int listener;
};
