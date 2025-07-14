#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class ArgParser {
public:
  ArgParser(int argc, char *argv[]);

  bool hasFlag(const std::string &flag) const;
  std::optional<std::string> getOption(const std::string &option) const;
  std::vector<std::string> getPositional() const;

private:
  std::unordered_map<std::string, std::string> options;
  std::vector<std::string> flags;
  std::vector<std::string> positional;
};
