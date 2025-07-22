#pragma once

#include <iostream>
#include <mutex>
#include <string_view>

namespace dcleaner {

enum class LogLevel { ERROR, WARNING, INFO, DEBUG };

// Глобальный уровень логирования определяется на этапе компиляции
#ifndef DCLEANER_LOG_LEVEL
#define DCLEANER_LOG_LEVEL LogLevel::INFO
#endif

#define LOG_SHOULD_LOG(level) (static_cast<int>(level) <= static_cast<int>(DCLEANER_LOG_LEVEL))

#define LOG_ERROR(logger, msg)                             \
  do {                                                     \
    if (LOG_SHOULD_LOG(dcleaner::LogLevel::ERROR)) \
      logger.error(msg);                                   \
  } while (0)

#define LOG_WARNING(logger, msg)                             \
  do {                                                       \
    if (LOG_SHOULD_LOG(dcleaner::LogLevel::WARNING)) \
      logger.warning(msg);                                   \
  } while (0)

#define LOG_INFO(logger, msg)                             \
  do {                                                    \
    if (LOG_SHOULD_LOG(dcleaner::LogLevel::INFO)) \
      logger.info(msg);                                   \
  } while (0)

#define LOG_DEBUG(logger, msg)                             \
  do {                                                     \
    if (LOG_SHOULD_LOG(dcleaner::LogLevel::DEBUG)) \
      logger.debug(msg);                                   \
  } while (0)

class Logger {
 public:
  explicit Logger(std::ostream& stream = std::cout);

  void log(LogLevel level, std::string_view message);

  void error(std::string_view message);
  void warning(std::string_view message);
  void info(std::string_view message);
  void debug(std::string_view message);

 private:
  std::ostream& stream_;
  std::mutex mutex_;

  static constexpr std::string_view level_to_string(LogLevel level) {
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

}  // namespace dcleaner::detail
