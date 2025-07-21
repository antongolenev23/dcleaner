#include "commands_output.hpp"

#include <iomanip>
#include <sstream>

namespace dcleaner {

const CategorySummaries& AnalyzeOutput::get_summaries() const {
  return summaries_;
}

const Errors& AnalyzeOutput::get_errors() const {
  return errors_;
}

void AnalyzeOutput::add_error(std::string path_str, std::string error) {
  errors_.emplace_back(std::move(path_str), std::move(error));
}

void AnalyzeOutput::increment_summary(FileCategory category,
                                      size_t entries_count,
                                      size_t entries_size_in_bytes_count) {
  summaries_[category].entries_count_ += entries_count;
  summaries_[category].entries_size_in_bytes_ += entries_size_in_bytes_count;
}

const CategorySummaries& DeleteOutput::get_summaries() const {
  return summaries_;
}

void DeleteOutput::increment_summary(FileCategory category,
                                     size_t entries_count,
                                     size_t entries_size_in_bytes_count) {
  summaries_[category].entries_count_ += entries_count;
  summaries_[category].entries_size_in_bytes_ += entries_size_in_bytes_count;
}

HelpOutput::HelpOutput(std::string&& help_info) : info_(std::move(help_info)) {}

const std::string& HelpOutput::get_info() const {
  return info_;
}

}  // namespace dcleaner