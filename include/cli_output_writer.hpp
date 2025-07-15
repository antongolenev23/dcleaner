#include <iostream>
#include "commands_result.hpp"

namespace dcleaner {

class CLIOutputWriter {
public:
    static void write_output(const CommandResult&, std::ostream&); 
};

} // namespace dcleaner
