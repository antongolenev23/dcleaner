#pragma once
#include <array>
#include <chrono>
#include <string>

namespace dcleaner::detail {

std::string join_strings(const std::vector<std::string>& parts, const std::string& delimiter);

bool match_glob_pattern(const std::string& pattern, const std::string& path);

bool match_part_with_braces(const std::string& pattern_part, const std::string& path_part);

bool matches_any_glob(const std::string& path, const std::vector<std::string>& globs);

template <std::size_t N>
bool matches_any_glob(const std::string& path, const std::array<std::string_view, N>& globs) {
    for (const auto& glob : globs) {
        if (match_glob_pattern(std::string(glob), path)) {
            return true;
        }
    }
    return false;
}

}  // namespace dcleaner::detail