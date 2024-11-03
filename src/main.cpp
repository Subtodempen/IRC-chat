#include "TCP/TcpFactory.hpp"

#include <iostream>

#include <chrono>
#include <thread>

//client main


int main(){
  TcpFactory clientFactory(SocketType::client);
  
  clientFactory.initSocketStream({"127.168.1.244", 1234});

  std::unique_ptr<TcpStream> clientStream = clientFactory.getStream();

  clientStream->tcpSend("Hello World!");

  BlockingQueue<std::string> blockingPool = clientStream->tcpRecvPool();
}
