#include "TcpFactory.hpp"
#include "TcpStream.hpp"
#include "peer/TcpClient.hpp"
#include <optional>
#include <iostream>

TcpFactory::TcpFactory(SocketType type) : _sessionSocket(type){
  if(type == SocketType::server)
    _server = new CreateServer();

  else
    _client = new CreateClient();

  _stream = std::make_unique<TcpStream>();
}

void TcpFactory::initSocketStream(clientOrServerInput portOrIp){ // change maybe optional pair?
  auto port = portOrIp.second; 

  if(_sessionSocket == SocketType::server){

    initServerSocket(port);

    _stream->setSocketFd(_server.value()->getSocketFd());
    _server = std::nullopt; // have just removed only member variable from _server so now its empty. 

  }else{
    auto optionalIp = portOrIp.first;

    initClientSocket(optionalIp.value(), port);
    
    _stream->setSocketFd(std::move(_client.value()->getSocketFd()));
    _client = std::nullopt;
  }

  _stream->startPooling();
}


void TcpFactory::initServerSocket(int port){
  auto& server = _server.value();

  server->setPort(port);
  server->initSocket();
  server->bindConnection();
  server->startListen(SERVER_LIMIT);
  server->acceptConnection(); // add functionality multiple streams 
}

void TcpFactory::initClientSocket(const std::string& ip, int port){
  auto& client = _client.value();  
  
  client->setIp(ip);
  client->setPort(port);
  client->createSocket();
  client->initConnection();
}


std::unique_ptr<TcpStream> TcpFactory::getStream(){
  return std::move(_stream);
}
