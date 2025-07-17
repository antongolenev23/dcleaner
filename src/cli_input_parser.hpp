#pragma once
#include "commands.hpp"
#include "logger.hpp"
#include <iostream>

namespace dcleaner {

class CLIInputParser {
public:
    CLIInputParser(const detail::Logger& logger)
        : logger_(logger) 
    {}

    static Command parse_input(std::istream&);

private:
    // Logger to create a Command objects
    const detail::Logger& logger_;
};

} // namespace dcleaner
