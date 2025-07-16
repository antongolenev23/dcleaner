#include <iostream>
#include "commands_output.hpp"

namespace dcleaner {

class CLIOutputWriter {
public:
    static void write_output(const CommandOutput&, std::ostream&); 
};

} // namespace dcleaner
