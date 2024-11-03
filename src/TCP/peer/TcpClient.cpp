#include "TcpClient.hpp"
#include <arpa/inet.h>
#include <memory>
#include <stdexcept>
#include <sys/socket.h>


CreateClient::CreateClient(){
  _serverInfo.sin_family = AF_INET;
}

void CreateClient::setIp(const std::string& ip){
  _serverInfo.sin_addr.s_addr = inet_addr(ip.c_str());
}

void CreateClient::setPort(int port){
  _serverInfo.sin_port = htons( port );
}

void CreateClient::createSocket(){
  int socketObj = socket(AF_INET , SOCK_STREAM , 0);
  setSocketFd(std::make_unique<int>(socketObj));

  if(*sockFd < 0)
    throw std::runtime_error("Can not create socket");
}


void CreateClient::initConnection(){
  int status = connect(*sockFd, reinterpret_cast<struct sockaddr*>(&_serverInfo), sizeof(_serverInfo));
  
  if(status < 0)
    throw std::runtime_error("Connection refused");
}
