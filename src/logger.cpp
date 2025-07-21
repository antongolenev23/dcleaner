#include <iomanip>
#include <logger.hpp>
#include <sstream>

namespace dcleaner::detail {

Logger::Logger(std::ostream& out) : ostream_(out) {}

void Logger::info(std::string_view message) {
  log("INFO", message);
}

void Logger::error(std::string_view message) {
  log("ERROR", message);
}

void Logger::warning(std::string_view message) {
  log("WARNING", message);
}

void Logger::fatal(std::string_view message) {
  log("FATAL", message);
}

void Logger::log(std::string_view level, std::string_view message) {
  const auto now = std::chrono::system_clock::now();
  const auto time = std::chrono::system_clock::to_time_t(now);

  const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch()) %
                  1000;
  std::stringstream ss;

  ss << std::put_time(std::localtime(&time), "[%Y-%m-%d %H:%M:%S.")
     << std::setfill('0') << std::setw(3) << ms.count() << "] ";

  ss << "[" << level << "] ";

  ss << message << '\n';

  {
    std::lock_guard<std::mutex> lock(log_mutex);
    ostream_ << ss.str() << std::endl;
  }
}

}  // namespace dcleaner::detail
