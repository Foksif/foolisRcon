#include "RconClient.hpp"
#include "utils/ArgParser.hpp"
#include "utils/ConfigManager.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  ArgParser args(argc, argv);

  if (!args.getPositional().empty() && args.getPositional()[0] == "add") {
    ConfigManager::interactiveAddProfile();
    return 0;
  }

  std::string profileName;
  if (auto opt = args.getOption("--profile"))
    profileName = *opt;
  else {
    std::cout << "Enter profile name: ";
    std::cin >> profileName;
  }

  auto profile = ConfigManager::loadProfile(profileName);
  if (!profile)
    return 1;

  RconClient client(profile->host, profile->port, profile->password);
  if (!client.connect()) {
    std::cerr << "Connection error.\n";
    return 1;
  }

  std::cin.ignore();
  std::string command;

  if (auto cmd = args.getOption("--cmd")) {
    std::cout << client.sendCommand(*cmd) << "\n";
    return 0;
  }

  while (true) {
    std::cout << "> ";
    std::getline(std::cin, command);
    if (command == "exit")
      break;
    std::cout << client.sendCommand(command) << "\n";
  }

  return 0;
}
