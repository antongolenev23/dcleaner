#pragma once

#include <iostream>
#include <mutex>
#include <string_view>
#include "../external/filesystem.hpp"

namespace dcleaner {

namespace fs = ghc::filesystem;

enum class LogLevel { ERROR, WARNING, INFO, DEBUG };

inline fs::path getLogsDir() {
    const char* home = std::getenv("HOME");
    if (home) {
        return fs::path(home) / ".local" / "share" / "dcleaner" / "logs";
    }
    return "/tmp/dcleaner/logs"; // fallback, если HOME вдруг не задан
}

inline const fs::path LOGS_DIR  = getLogsDir();
inline const fs::path LOGS_FILE = LOGS_DIR / "logs.txt";

// Глобальный уровень логирования определяется на этапе компиляции
#ifndef DCLEANER_LOG_LEVEL
#define DCLEANER_LOG_LEVEL dcleaner::LogLevel::INFO
#endif

#define LOG_SHOULD_LOG(level) (static_cast<int>(level) <= static_cast<int>(DCLEANER_LOG_LEVEL))

#define LOG_ERROR(logger, msg)                     \
  do {                                             \
    if (LOG_SHOULD_LOG(dcleaner::LogLevel::ERROR)) \
      logger.error(msg);                           \
  } while (0)

#define LOG_WARNING(logger, msg)                     \
  do {                                               \
    if (LOG_SHOULD_LOG(dcleaner::LogLevel::WARNING)) \
      logger.warning(msg);                           \
  } while (0)

#define LOG_INFO(logger, msg)                     \
  do {                                            \
    if (LOG_SHOULD_LOG(dcleaner::LogLevel::INFO)) \
      logger.info(msg);                           \
  } while (0)

#define LOG_DEBUG(logger, msg)                     \
  do {                                             \
    if (LOG_SHOULD_LOG(dcleaner::LogLevel::DEBUG)) \
      logger.debug(msg);                           \
  } while (0)

class Logger {
 public:
  explicit Logger(std::ostream& stream = std::cout);

  virtual void log(LogLevel level, std::string_view message);

  void error(std::string_view message);
  void warning(std::string_view message);
  void info(std::string_view message);
  void debug(std::string_view message);

 private:
  std::ostream& stream_;
  std::mutex mutex_;

  static constexpr std::string level_to_string(LogLevel level) {
    switch (level) {
      case LogLevel::ERROR:
        return "[ERROR]";
      case LogLevel::WARNING:
        return "[WARNING]";
      case LogLevel::INFO:
        return "[INFO]";
      case LogLevel::DEBUG:
        return "[DEBUG]";
      default:
        return "[UNKNOWN]";
    }
  }

  static constexpr bool should_log(LogLevel level) {
    return static_cast<int>(level) <= static_cast<int>(DCLEANER_LOG_LEVEL);
  }
};

}  // namespace dcleaner
