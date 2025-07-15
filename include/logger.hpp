#include <fstream>
#include <string_view>

namespace dcleaner {

namespace detail {

class Logger {
public:
    void log_info(std::string_view);
    void log_error(std::string_view);
    void log_warning(std::string_view);
    void log_delete(std::string_view);

private:
    std::ofstream& log_file_path_;
};

} // namespace detail

} // namespace dcleaner