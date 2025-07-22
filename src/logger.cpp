#include "logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <mutex>
#include <sstream>

namespace dcleaner {

Logger::Logger(std::ostream& out) : stream_(out) {}

void Logger::log(LogLevel level, std::string_view message) {
  if (!should_log(level)) return;

  const auto now = std::chrono::system_clock::now();
  const auto time = std::chrono::system_clock::to_time_t(now);
  const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch()) %
                  1000;

  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "[%Y-%m-%d %H:%M:%S.")
     << std::setfill('0') << std::setw(3) << ms.count() << "] ";

  ss << "[" << level_to_string(level) << "] ";
  ss << message << '\n';

  {
    std::lock_guard<std::mutex> lock(mutex_);
    stream_ << ss.str() << std::flush;
  }
}

void Logger::error(std::string_view message) {
  log(LogLevel::ERROR, message);
}

void Logger::warning(std::string_view message) {
  log(LogLevel::WARNING, message);
}

void Logger::info(std::string_view message) {
  log(LogLevel::INFO, message);
}

void Logger::debug(std::string_view message) {
  log(LogLevel::DEBUG, message);
}

}  // namespace dcleaner::detail
