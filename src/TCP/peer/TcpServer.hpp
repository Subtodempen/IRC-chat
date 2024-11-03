#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdexcept>

#include "../SocketHolder.hpp"

class CreateServer : public  BaseSocket{
public:
  CreateServer();
  
  void initSocket();
  void bindConnection(); // add state so their called in order fsm
  void startListen(const int queueLimit);
  int acceptConnection();
  
  void setPort(int port);
private:
  struct sockaddr_in _serverInfo;
};
