#include "ArgParser.hpp"
#include <algorithm>
#include <string>

static bool startsWith(const std::string &str, const std::string &prefix) {
  return str.size() >= prefix.size() &&
         str.compare(0, prefix.size(), prefix) == 0;
}

ArgParser::ArgParser(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (startsWith(arg, "--")) {
      std::string key = arg;
      std::string value;
      if (i + 1 < argc && !startsWith(argv[i + 1], "--")) {
        value = argv[i + 1];
        ++i;
      }
      options[key] = value;
    } else if (startsWith(arg, "-")) {
      flags.push_back(arg);
    } else {
      positional.push_back(arg);
    }
  }
}

bool ArgParser::hasFlag(const std::string &flag) const {
  if (std::find(flags.begin(), flags.end(), flag) != flags.end())
    return true;
  if (options.find(flag) != options.end())
    return true;
  return false;
}

std::optional<std::string>
ArgParser::getOption(const std::string &option) const {
  auto it = options.find(option);
  if (it != options.end())
    return it->second;
  return std::nullopt;
}

std::vector<std::string> ArgParser::getPositional() const { return positional; }
