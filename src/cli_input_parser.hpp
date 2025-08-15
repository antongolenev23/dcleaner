#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "commands.hpp"
#include "logger.hpp"

namespace dcleaner {

class CLIInputParser {
 public:
  CLIInputParser(Logger& logger) : logger_(logger) {}

  ParsingResult parse_input(std::istream& input_stream);

 private:
  std::vector<std::string> split_stream(std::istream& is);
  // Logger to create a Command objects
  Logger& logger_;
};

}  // namespace dcleaner
