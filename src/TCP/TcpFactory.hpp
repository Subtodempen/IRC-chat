#pragma once

#include <memory>
#include <optional>
#include <variant>

#include "peer/TcpClient.hpp"
#include "peer/TcpServer.hpp"

#include "TcpStream.hpp"

#define SERVER_LIMIT 15

enum class SocketType{
  client, 
  server
};

using clientOrServerInput = std::pair<std::optional<std::string>, int>;


class TcpFactory{
public:
  TcpFactory(SocketType);
  
  void initSocketStream(clientOrServerInput);
  std::unique_ptr<TcpStream> getStream();

private:
  std::optional<CreateClient*> _client;
  std::optional<CreateServer*> _server;

  std::unique_ptr<TcpStream> _stream;

  const SocketType _sessionSocket;

  void initServerSocket(int port);
  void initClientSocket(const std::string& ip, int port);
};
