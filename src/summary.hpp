#pragma once
#include <string>

namespace dcleaner {

enum class FileCategory {
    INACTIVE    = 1,
    EMPTY       = 1 << 1,
    CACHE       = 1 << 2,
    TEMPORARY   = 1 << 3, 
};

enum class DeletePolicy {
    FORCE = 1 << 4,
};

struct CategorySummary {;
    uint64_t files_total_size_in_bytes_;
    size_t files_count_;
    FileCategory category_;

    explicit CategorySummary(FileCategory category)
        : category_(category)
        , files_total_size_in_bytes_(0)
        , files_count_(0)
    {}
};
 
} // namespace dcleaner


