#pragma once
#include <string>

namespace dcleaner {

namespace detail {

struct CategorySummary {
    std::string category_name_;
    uint64_t files_total_size_in_bytes_;
    size_t files_count_;
};

} // namspace detail
 
} // namespace dcleaner


