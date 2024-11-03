#include "TcpServer.hpp"

CreateServer::CreateServer(){
  _serverInfo.sin_port = AF_INET;
  _serverInfo.sin_addr.s_addr = INADDR_ANY;
}


void CreateServer::initSocket(){
  int socketObj = socket(AF_INET, SOCK_STREAM, 0);
  setSocketFd(std::make_unique<int>(socketObj));
  
  if(*sockFd == -1)
    throw std::runtime_error("Socket not created");
}

void CreateServer::bindConnection(){
  int res = bind(*sockFd, reinterpret_cast<struct sockaddr*>(&_serverInfo), sizeof(_serverInfo));

  if(res < 0)
    throw std::runtime_error("Can not bind");
}


void CreateServer::startListen(const int queueLimit){
  listen(*sockFd, queueLimit);
}

int CreateServer::acceptConnection(){
  struct sockaddr_in clientInfo;
  int clientInfoLen = sizeof(struct sockaddr_in);
  int clientSock;

  clientSock = accept(*sockFd, reinterpret_cast<struct sockaddr*>(&clientInfo), (socklen_t*)&clientInfoLen);

  return clientSock;
}


void CreateServer::setPort(int port){
  _serverInfo.sin_port = htons(port); 
}
