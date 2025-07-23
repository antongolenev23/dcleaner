#include "commands_output.hpp"
#include "logger.hpp"

#include <iomanip>
#include <sstream>

namespace dcleaner {

const CategorySummaries& CommandOutput::get_summaries() const {
  return summaries_;
}

const Errors& CommandOutput::get_errors() const {
  return errors_;
}

void CommandOutput::add_error(std::string path_str, std::string error) {
  errors_.emplace_back(std::move(path_str), std::move(error));
}

void CommandOutput::increment_summary(FileCategory category,
                                      size_t entries_count,
                                      size_t entries_size_in_bytes_count) {
  summaries_[category].entries_count_ += entries_count;
  summaries_[category].entries_size_in_bytes_ += entries_size_in_bytes_count;
}
}  // namespace dcleaner