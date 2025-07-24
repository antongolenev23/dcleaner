#pragma once
#include <iostream>

#include "commands.hpp"
#include "logger.hpp"

namespace dcleaner {

class CLIInputParser {
 public:
  CLIInputParser(const Logger& logger) : logger_(logger) {}

  static Command parse_input(std::istream&);

 private:
  // Logger to create a Command objects
  const Logger& logger_;
};

}  // namespace dcleaner
