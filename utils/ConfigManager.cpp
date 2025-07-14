#include "ConfigManager.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::optional<ServerProfile>
ConfigManager::loadProfile(const std::string &name) {
  std::string configDir = std::string(getenv("HOME")) + "/.config/foolisRcon";
  std::string configPath = configDir + "/servers.json";

  if (!std::filesystem::exists(configDir))
    std::filesystem::create_directories(configDir);

  if (!std::filesystem::exists(configPath)) {
    json defaultData = {
        {"local",
         {{"host", "127.0.0.1"}, {"port", 25575}, {"password", "changeme"}}}};
    std::ofstream out(configPath);
    out << defaultData.dump(4);
    out.close();
    std::cout << "Configuration file created: " << configPath << "\n";
    return std::nullopt;
  }

  std::ifstream file(configPath);
  json j;
  try {
    file >> j;
  } catch (...) {
    std::cerr << "Error read JSON.\n";
    return std::nullopt;
  }

  if (!j.contains(name)) {
    std::cerr << "Profile '" << name << "' not a found. Profiles:\n";
    for (auto &el : j.items())
      std::cout << " - " << el.key() << "\n";
    return std::nullopt;
  }

  return ServerProfile{j[name]["host"], j[name]["port"], j[name]["password"]};
}

void ConfigManager::interactiveAddProfile() {
  std::string configDir = std::string(getenv("HOME")) + "/.config/foolisRcon";
  std::string configPath = configDir + "/servers.json";

  if (!std::filesystem::exists(configDir))
    std::filesystem::create_directories(configDir);

  json j;
  std::ifstream in(configPath);
  if (in)
    in >> j;
  in.close();

  std::string name, host, password;
  int port;
  std::cout << "Enter profile name: ";
  std::cin >> name;
  std::cout << "Ip or host: ";
  std::cin >> host;
  std::cout << "Port: ";
  std::cin >> port;
  std::cout << "Password: ";
  std::cin >> password;

  j[name] = {{"host", host}, {"port", port}, {"password", password}};
  std::ofstream out(configPath);
  out << j.dump(4);
  std::cout << "Profile '" << name << "' added.\n";
}
