#pragma once

#include <memory>
#include <utility>


class BaseSocket{
public:

  std::unique_ptr<int> getSocketFd(){
    return std::move(sockFd);
  }

  void setSocketFd(std::unique_ptr<int>&& newSockFd){
    sockFd = std::move(newSockFd);
  }

protected:
  std::unique_ptr<int> sockFd;
};
