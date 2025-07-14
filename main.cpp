#include "RconClient.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

using json = nlohmann::json;

struct ServerProfile {
  std::string host;
  int port;
  std::string password;
};

std::optional<ServerProfile> loadProfile(const std::string &name) {
  std::string configDir = std::string(getenv("HOME")) + "/.config/foolisRcon";
  std::string configPath = configDir + "/servers.json";

  if (!std::filesystem::exists(configDir)) {
    std::filesystem::create_directories(configDir);
    std::cout << "Created config directory: " << configDir << "\n";
  }

  if (!std::filesystem::exists(configPath)) {
    json defaultData = {
        {"local",
         {{"host", "127.0.0.1"}, {"port", 25575}, {"password", "changeme"}}}};

    std::ofstream out(configPath);
    out << defaultData.dump(4);
    out.close();

    std::cout << "Created default config: " << configPath << "\n";
    std::cout << "Edit it to add your server profiles.\n";
    return std::nullopt;
  }

  std::ifstream file(configPath);
  json j;
  try {
    file >> j;
  } catch (const std::exception &e) {
    std::cerr << "Invalid JSON: " << e.what() << "\n";
    return std::nullopt;
  }

  if (!j.contains(name)) {
    std::cerr << "Profile '" << name << "' not found.\nAvailable profiles:\n";
    for (auto &el : j.items()) {
      std::cout << " - " << el.key() << "\n";
    }
    return std::nullopt;
  }

  return ServerProfile{j[name]["host"], j[name]["port"], j[name]["password"]};
}

int main() {
  std::string profileName;
  std::cout << "Enter profile name: ";
  std::cin >> profileName;

  auto profile = loadProfile(profileName);
  if (!profile)
    return 1;

  RconClient client(profile->host, profile->port, profile->password);
  if (!client.connect()) {
    std::cerr << "Failed to connect or authenticate.\n";
    return 1;
  }

  std::cin.ignore();
  std::string command;
  while (true) {
    std::cout << "> ";
    std::getline(std::cin, command);
    if (command == "exit")
      break;

    std::string response = client.sendCommand(command);
    std::cout << response << "\n";
  }

  return 0;
}
