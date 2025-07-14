#pragma once
#include <string>

class RconClient {
public:
  RconClient(const std::string &host, int port, const std::string &password);
  ~RconClient();

  bool connect();
  std::string sendCommand(const std::string &command);

private:
  std::string host;
  int port;
  std::string password;
  int socket_fd;
  int request_id;

  bool authenticate();
  std::string buildPacket(int type, const std::string &payload);
  std::string readResponse();
};
