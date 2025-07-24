#pragma once
#include <string>
#include <array>
#include <chrono>

namespace dcleaner::detail {

std::string join_strings(std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end,
                         const std::string& delimiter);

bool match_glob_pattern(const std::string& pattern, const std::string& path);

template <std::size_t N>
bool matches_any_glob(const std::string& path, const std::array<std::string_view, N>& globs) {
  for (const auto& pattern : globs) {
    if (match_glob_pattern(std::string(pattern), path)) {
      return true;
    }
  }
  return false;
}

}  // namespace dcleaner::detail