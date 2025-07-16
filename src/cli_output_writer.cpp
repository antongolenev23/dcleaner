#include "cli_output_writer.hpp"
#include <sstream>
#include <iomanip>

namespace dcleaner {

namespace detail {

std::string format_size(uint64_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB", "PB"};
    size_t unit_index = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024 && unit_index < std::size(units) - 1) {
        size /= 1024;
        ++unit_index;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(size < 10 ? 1 : 0) << size << ' ' << units[unit_index];
    return oss.str();
}

} // namespace detail

} // namespace dcleaner