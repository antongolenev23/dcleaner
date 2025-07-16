#pragma once
#include <fstream>
#include <string_view>

namespace dcleaner {

namespace detail {

class Logger {
public:
    Logger(std::ostream&);

    void info(std::string_view);
    void error(std::string_view);
    void warning(std::string_view);
    void fatal(std::string_view);

private:
    std::ostream& ostream_;
};

} // namespace detail

} // namespace dcleaner