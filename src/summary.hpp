#pragma once
#include <string>

namespace dcleaner {

enum class FileCategory {
  INACTIVE = 1,
  EMPTY = 1 << 1,
};

enum class DeletePolicy {
  FORCE = 1 << 2,
};

struct CategorySummary {
  size_t entries_size_in_bytes_;
  size_t entries_count_;

  CategorySummary() : entries_size_in_bytes_(0), entries_count_(0) {}
};

}  // namespace dcleaner
