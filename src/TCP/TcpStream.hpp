#pragma once

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <errno.h>
#include <cstring>

#include "../ThreadPool/BlockingQueue.hpp"
#include "SocketHolder.hpp"

#define IRC_MAX_MSG_LENGTH 512


class TcpStream : public BaseSocket{
public:
  TcpStream();
  ~TcpStream();

  void tcpSend(const std::string& msg);
  BlockingQueue<std::string> tcpRecvPool(); 

  void startPooling(); // create its own thread for pool
  void stopPooling(); // send data via a pipe to the select call, this will stop it,  

private:
  BlockingQueue<std::string> recvPool;
  std::thread recvThread;

  int pipeFd[2];
  std::atomic<bool> run;
  
  void pool(); // for safety it can only access the thread safe object
};
