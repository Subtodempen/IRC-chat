#include "TcpStream.hpp"
#include <exception>
#include <functional>
#include <stdexcept>
#include <string>

#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>


TcpStream::TcpStream(){
  pipe(pipeFd);// change 
}

TcpStream::~TcpStream(){ // deconstruct thread , shutdown socket  
  stopPooling();

  shutdown(*sockFd, SHUT_RDWR);
  close(*sockFd); 
} 

BlockingQueue<std::string> TcpStream::tcpRecvPool(){
  return recvPool;
}

void TcpStream::tcpSend(const std::string& msg){
  const char* cStringMsg = msg.c_str();
  unsigned int msgLen = msg.length();

  if(send(*sockFd, cStringMsg, msgLen, MSG_DONTWAIT) < 0){
      const char* error = strerror(errno);
      throw std::string(error);
  }
}


void TcpStream::startPooling(){
  //create a thread for pool 
  run = true;
  recvThread = std::thread(&TcpStream::pool, this);
}


void TcpStream::stopPooling(){
  const char* msg = "1";
  write(pipeFd[1], msg, 1);

  run = false;
  recvThread.join();
}


// constantly running in the background recvs blocks so thread is not using that many recourses,
// when msg is inputted it immediately gets appended to the localRecvPool 
// TODO notfiy some object that data has been received
void TcpStream::pool(){  
  if (!sockFd) 
    throw std::runtime_error("Socket not initialized");

  fd_set readFd;

  FD_ZERO(&readFd);
  FD_SET(*sockFd, &readFd);
  FD_SET(pipeFd[0], &readFd);

  int maxFd = *sockFd > pipeFd[0] ? *sockFd : pipeFd[0];

  while(run){
    int activity = select(maxFd + 1, &readFd, NULL, NULL, NULL);

    if(activity < 0)
      throw std::runtime_error("Can not receive");
    
    if(FD_ISSET(pipeFd[0], &readFd))
      break;

    else if(FD_ISSET(*sockFd, &readFd)){
      char msg[IRC_MAX_MSG_LENGTH];
      recv(*sockFd, msg, IRC_MAX_MSG_LENGTH, 0); // blocks so, run can not be evaluated.
      recvPool.enqueue(std::string(msg));     // when message is recv append it to the pool
    }
  }
}
