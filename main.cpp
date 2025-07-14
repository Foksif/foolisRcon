#include "RconClient.hpp"
#include "utils/ArgParser.hpp"
#include "utils/ConfigManager.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  ArgParser args(argc, argv);

  if (args.hasFlag("--help") || args.hasFlag("-h")) {
    std::cout << R"(foolisRcon — Simple RCON client

Usage:
  foolisRcon [options] [positional arguments]

Options:
  --profile <name>      Load a saved profile from ~/.config/foolisRcon/servers.json
  --cmd <command>       Execute a single command and exit
  --list-profiles       List all profiles
  --help, -h            Show this help message

Positional commands:
  add                   Launch interactive mode to create a new profile

Examples:
  foolisRcon --profile local
      Connect to the 'local' profile and start interactive command mode.

  foolisRcon --profile local --cmd "say hello"
      Send a one-time command to the server and exit.

  foolisRcon add
      Enter interactive mode to add a new server profile.

Config file:
  ~/.config/foolisRcon/servers.json
      Stores all RCON connection profiles in JSON format.
)";
    return 0;
  }

  if (!args.getPositional().empty() && args.getPositional()[0] == "add") {
    ConfigManager::interactiveAddProfile();
    return 0;
  }

  if (args.hasFlag("--list-profiles")) {
    ConfigManager::listProfiles();
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
