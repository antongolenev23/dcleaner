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
  virtual const CategorySummaries& get_summaries() const;
  virtual const Errors& get_errors() const;
  virtual void add_error(std::string path_str, std::string error);
  virtual void increment_summary(FileCategory category, size_t entries_count, size_t entries_size_in_bytes_count);

 private:
  Errors errors_;
  CategorySummaries summaries_;
};

class AnalyzeOutput : public CommandOutput {};

class DeleteOutput : public CommandOutput {};

}  // namespace dcleaner
