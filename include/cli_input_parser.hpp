#include "commands.hpp"
#include <iostream>

namespace dcleaner {

class CLIInputReader {
public:
    static Command parse_input(std::istream&); 
};

} // namespace dcleaner
