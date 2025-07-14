#include "RconClient.hpp"
#include "utils/Network.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

enum RconPacketType {
  AUTH = 3,
  AUTH_RESPONSE = 2,
  COMMAND = 2,
  RESPONSE_VALUE = 0
};

RconClient::RconClient(const std::string &host, int port,
                       const std::string &password)
    : host(host), port(port), password(password), socket_fd(-1), request_id(0) {
}

RconClient::~RconClient() {
  if (socket_fd != -1) {
    close(socket_fd);
  }
}

bool RconClient::connect() {
  socket_fd = createSocket(host, port);
  if (socket_fd == -1)
    return false;

  return authenticate();
}

bool RconClient::authenticate() {
  std::string packet = buildPacket(AUTH, password);
  ::send(socket_fd, packet.data(), packet.size(), 0);
  std::string response = readResponse();
  return !response.empty();
}

std::string RconClient::sendCommand(const std::string &command) {
  std::string packet = buildPacket(COMMAND, command);
  ::send(socket_fd, packet.data(), packet.size(), 0);
  return readResponse();
}

std::string RconClient::buildPacket(int type, const std::string &payload) {
  request_id++;
  int size = 4 + 4 + payload.size() + 2;
  std::string packet;
  packet.resize(4 + size);

  int32_t len = size;
  int32_t id = request_id;
  int32_t t = type;

  std::memcpy(&packet[0], &len, 4);
  std::memcpy(&packet[4], &id, 4);
  std::memcpy(&packet[8], &t, 4);
  std::memcpy(&packet[12], payload.c_str(), payload.size());
  packet[size + 3] = '\0';
  packet[size + 2] = '\0';

  return packet;
}

std::string RconClient::readResponse() {
  char buffer[4096];
  ssize_t len = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
  if (len <= 0)
    return "";

  buffer[len] = '\0';
  return std::string(buffer + 12, len - 14);
}
