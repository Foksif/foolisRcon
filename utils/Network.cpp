#include "Network.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>

int createSocket(const std::string &host, int port) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    return -1;

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);

  hostent *he = gethostbyname(host.c_str());
  if (!he)
    return -1;

  std::memcpy(&server_addr.sin_addr, he->h_addr_list[0], he->h_length);

  if (connect(sock, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    close(sock);
    return -1;
  }

  return sock;
}
