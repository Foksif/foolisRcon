#pragma once
#include <optional>
#include <string>

struct ServerProfile {
  std::string host;
  int port;
  std::string password;
};

class ConfigManager {
public:
  static std::optional<ServerProfile> loadProfile(const std::string &name);
  static void interactiveAddProfile();
};
