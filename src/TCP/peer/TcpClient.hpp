#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

#include "../SocketHolder.hpp"

class CreateClient : public BaseSocket{
public:
  CreateClient();
  
  void createSocket();
  void initConnection();
  
  void setIp(const std::string&);
  void setPort(int);
private:
  struct sockaddr_in _serverInfo;
};
