#include "glob_match_check.hpp"
#include <fnmatch.h>

namespace dcleaner::detail {

std::string join_strings(const std::vector<std::string>& parts, const std::string& delimiter) {
    std::string result;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i != 0) result += delimiter;
        result += parts[i];
    }
    return result;
}

bool match_part_with_braces(const std::string& pattern_part, const std::string& path_part) {
    size_t brace_start = pattern_part.find('{');
    if (brace_start == std::string::npos) {
        return fnmatch(pattern_part.c_str(), path_part.c_str(), 0) == 0;
    }

    size_t brace_end = pattern_part.find('}', brace_start);
    if (brace_end == std::string::npos) {
        return fnmatch(pattern_part.c_str(), path_part.c_str(), 0) == 0;
    }

    std::string prefix = pattern_part.substr(0, brace_start);
    std::string suffix = pattern_part.substr(brace_end + 1);
    std::string alternatives_str = pattern_part.substr(brace_start + 1, brace_end - brace_start - 1);

    std::istringstream iss(alternatives_str);
    std::string alternative;
    while (std::getline(iss, alternative, ',')) {
        std::string full_pattern = prefix + alternative + suffix;
        if (fnmatch(full_pattern.c_str(), path_part.c_str(), 0) == 0) {
            return true;
        }
    }

    return false;
}

bool match_glob_pattern(const std::string& pattern, const std::string& path) {
    if (pattern == path) {
        return true;
    }

    std::vector<std::string> pattern_parts;
    std::istringstream pattern_stream(pattern);
    std::string part;
    while (std::getline(pattern_stream, part, '/')) {
        pattern_parts.push_back(part);
    }

    std::vector<std::string> path_parts;
    std::istringstream path_stream(path);
    while (std::getline(path_stream, part, '/')) {
        path_parts.push_back(part);
    }

    size_t pattern_idx = 0;
    size_t path_idx = 0;

    while (pattern_idx < pattern_parts.size() && path_idx < path_parts.size()) {
        const std::string& pattern_part = pattern_parts[pattern_idx];

        if (pattern_part == "**") {
            if (pattern_idx == pattern_parts.size() - 1) {
                return true;
            }

            for (size_t i = path_idx; i <= path_parts.size(); ++i) {
                std::vector<std::string> remaining_pattern(pattern_parts.begin() + pattern_idx + 1, pattern_parts.end());
                std::vector<std::string> remaining_path(path_parts.begin() + i, path_parts.end());
                if (match_glob_pattern(join_strings(remaining_pattern, "/"), join_strings(remaining_path, "/"))) {
                    return true;
                }
            }
            return false;
        } else {
            if (match_part_with_braces(pattern_part, path_parts[path_idx])) {
                pattern_idx++;
                path_idx++;
            } else {
                return false;
            }
        }
    }

    return pattern_idx == pattern_parts.size() && path_idx == path_parts.size();
}

bool matches_any_glob(const std::string& path, const std::vector<std::string>& globs) {
    for (const auto& pattern : globs) {
        if (match_glob_pattern(pattern, path)) {
            return true;
        }
    }
    return false;
}

} // namespace dcleaner::detail