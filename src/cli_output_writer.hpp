#pragma once
#include <iostream>
#include "commands_output.hpp"

namespace dcleaner {

class CLIOutputWriter {
public:
    static void write_output(const CommandOutput&, std::ostream&); 
};

namespace detail {

std::string format_size(uint64_t bytes);

} // namespace detail

} // namespace dcleaner
