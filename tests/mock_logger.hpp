#pragma once
#include "logger.hpp"

class MockLogger : public dcleaner::Logger {
 public:
  void log(dcleaner::LogLevel level, std::string_view message) override {}
};
