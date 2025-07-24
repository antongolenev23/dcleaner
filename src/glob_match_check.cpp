#include "glob_match_check.hpp"
#include <fnmatch.h>

namespace dcleaner::detail {

// Вспомогательная функция для соединения строк
std::string join_strings(std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end,
                         const std::string& delimiter) {
  std::string result;
  for (auto it = begin; it != end; ++it) {
    if (!result.empty()) {
      result += delimiter;
    }
    result += *it;
  }
  return result;
}

bool match_glob_pattern(const std::string& pattern, const std::string& path) {
  // Если паттерн точь-в-точь совпадает с путем
  if (pattern == path) {
    return true;
  }

  // Разбиваем паттерн и путь на компоненты
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

  // Индексы для текущего компонента в паттерне и пути
  size_t pattern_idx = 0;
  size_t path_idx = 0;

  while (pattern_idx < pattern_parts.size() && path_idx < path_parts.size()) {
    const std::string& pattern_part = pattern_parts[pattern_idx];

    if (pattern_part == "**") {
      // Рекурсивное совпадение: пропускаем любое количество каталогов
      if (pattern_idx == pattern_parts.size() - 1) {
        // ** в конце паттерна - совпадает с остатком пути
        return true;
      }

      // Пробуем совместить оставшийся паттерн с каждой возможной позицией в пути
      for (size_t i = path_idx; i <= path_parts.size(); ++i) {
        if (match_glob_pattern(join_strings(pattern_parts.begin() + pattern_idx + 1, pattern_parts.end(), "/"),
                               join_strings(path_parts.begin() + i, path_parts.end(), "/"))) {
          return true;
        }
      }
      return false;
    } else if (pattern_part == "*" || fnmatch(pattern_part.c_str(), path_parts[path_idx].c_str(), 0) == 0) {
      // Обычное совпадение или звездочка
      pattern_idx++;
      path_idx++;
    } else {
      return false;
    }
  }

  // Если дошли до конца и паттерна, и пути - совпадение
  return pattern_idx == pattern_parts.size() && path_idx == path_parts.size();
}

} // namespace dcleaner::detail