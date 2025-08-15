#pragma once
#include <iostream>
#include <string_view>

#include "commands.hpp"
#include "commands_output.hpp"

namespace dcleaner {

class CLIOutputWriter {
 public:
  void write_output(const ExecuteResult&, std::ostream&);
};

}  // namespace dcleaner

namespace dcleaner::detail {

namespace ansi {

constexpr std::string_view reset = "\033[0m";
constexpr std::string_view red = "\033[31m";
constexpr std::string_view green = "\033[32m";
constexpr std::string_view yellow = "\033[33m";
constexpr std::string_view cyan = "\033[36m";

}  // namespace ansi

std::string format_size(uint64_t bytes);

}  // namespace dcleaner::detail