#pragma once
#include <fstream>
#include <mutex>
#include <string_view>

namespace dcleaner::detail {

namespace {
std::mutex log_mutex;
}

class Logger {
 public:
  Logger(std::ostream&);

  void info(std::string_view);
  void error(std::string_view);
  void warning(std::string_view);
  void fatal(std::string_view);

 private:
  void log(std::string_view level, std::string_view message);

  std::ostream& ostream_;
};

}  // namespace dcleaner::detail