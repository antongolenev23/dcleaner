#pragma once
#include <iostream>

#include "commands_output.hpp"
#include "commands.hpp"

namespace dcleaner {

class CLIOutputWriter {
 public:
  static void write_output(const ExecuteResult&, std::ostream&);
};

}  // namespace dcleaner

namespace dcleaner::detail {

std::string format_size(uint64_t bytes);

}  // namespace dcleaner::detail