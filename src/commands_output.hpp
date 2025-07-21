#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "summary.hpp"

namespace dcleaner {

using CategorySummaries = std::map<FileCategory, CategorySummary>;

struct ErrorInfo {
  std::string path_str_;
  std::string error_info_;
};

using Errors = std::vector<ErrorInfo>;

class CommandOutput {
 public:
  virtual ~CommandOutput() = default;
};

class AnalyzeOutput : public CommandOutput {
 public:
  const CategorySummaries& get_summaries() const;
  const Errors& get_errors() const;
  void add_error(std::string path_str, std::string error);
  void increment_summary(FileCategory category, size_t entries_count,
                         size_t entries_size_in_bytes_count);

 private:
  Errors errors_;
  CategorySummaries summaries_;
};

class DeleteOutput : public CommandOutput {
 public:
  const CategorySummaries& get_summaries() const;
  void increment_summary(FileCategory category, size_t entries_count,
                         size_t entries_size_in_bytes_count);

 private:
  CategorySummaries summaries_;
};

class HelpOutput : public CommandOutput {
 public:
  explicit HelpOutput(std::string&&);
  const std::string& get_info() const;

 private:
  std::string info_;
};

}  // namespace dcleaner
