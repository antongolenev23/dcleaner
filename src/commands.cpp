#include "commands.hpp"

#include <fnmatch.h>
#include <sys/stat.h>

#include <chrono>

#include "dir_blacklist.hpp"
#include "dir_whitelist_cache.hpp"
#include "dir_whitelist_temp.hpp"

namespace dcleaner {

Command::Command(detail::Logger& logger) : logger_(logger) {}

Analyze::Analyze(detail::Logger& logger, detail::UserParameters&& parameters)
    : Command{logger}, parameters_(parameters) {}

ExecuteResult Analyze::execute() const {
  AnalyzeOutput output;
  // todo logging
  for (const fs::path& root : parameters_.get_paths()) {
    try {
      if (!fs::exists(root)) {
        output.add_error(root.string(), "Path does not exists");
        continue;
      }

      if (fs::is_directory(root)) {
        for (const auto& entry : fs::recursive_directory_iterator(
                 root, fs::directory_options::skip_permission_denied)) {
          try {
            if (entry.is_symlink()) {
              continue;
            }

            const auto& path = entry.path();
            std::string path_str = path.string();

            if (detail::matches_any_glob(path_str, detail::BLACKLIST)) {
              continue;
            }

            struct stat info;
            if (stat(path_str.c_str(), &info) != 0) {
              continue;
            }

            if (S_ISREG(info.st_mode)) {
              categorize_file(path_str, info, output);
            } else if (S_ISDIR(info.st_mode)) {
              categorize_folder(path, info, output);
            }

          } catch (const fs::filesystem_error& e) {
            continue;
          }
        }
      } else if (fs::is_regular_file(root)) {
        std::string path_str = root.string();

        struct stat info;
        if (stat(path_str.c_str(), &info) != 0) {
          // todo logging
          continue;
        }
        categorize_file(path_str, info, output);
      }
    } catch (const fs::filesystem_error& e) {
      output.add_error(root.string(), e.what());
    }
  }

  return std::make_unique<CommandOutput>(std::move(output));
}

void Analyze::categorize_file(const std::string& path_str,
                              const struct stat& info,
                              AnalyzeOutput& output) const {
  if (parameters_.has_flag(FileCategory::INACTIVE)) {
    auto last_access_time = detail::get_last_access_approx(info);
    auto time_now = std::chrono::system_clock::now();

    auto duration = last_access_time < time_now ? time_now - last_access_time
                                                : last_access_time - time_now;

    auto days = duration_cast<std::chrono::days>(duration).count();
    size_t days_passed = static_cast<size_t>(days);

    if (days_passed >= parameters_.get_inactive_days_count()) {
      output.increment_summary(FileCategory::INACTIVE, 1, info.st_size);
      return;
    }
  }
  if (parameters_.has_flag(FileCategory::CACHE)) {
    if (detail::matches_any_glob(path_str, detail::CACHE_WHITELIST)) {
      output.increment_summary(FileCategory::CACHE, 1, info.st_size);
      return;
    }
  }
  if (parameters_.has_flag(FileCategory::TEMPORARY)) {
    if (detail::matches_any_glob(path_str, detail::TEMP_FILES_WHITELIST)) {
      output.increment_summary(FileCategory::TEMPORARY, 1, info.st_size);
    }
  }
}

void Analyze::categorize_folder(const ghc::filesystem::path& path,
                                const struct stat& info,
                                AnalyzeOutput& output) const {
  if (parameters_.has_flag(FileCategory::EMPTY)) {
    bool is_empty = fs::is_empty(path);
    if (is_empty) {
      output.increment_summary(FileCategory::EMPTY, 1, info.st_size);
    }
  }
}

Delete::Delete(detail::Logger& logger, detail::UserParameters&& parameters)
    : Command{logger}, parameters_(parameters) {}

ExecuteResult Delete::execute() const {
  // todo
}

ExecuteResult Help::execute() const {
  // todo help text
  std::unique_ptr<CommandOutput> help_output =
      std::make_unique<HelpOutput>("dummy text");
  return help_output;
}

ExecuteResult Exit::execute() const {
  return std::unexpected(ExitSignal{});
}

}  // namespace dcleaner

namespace dcleaner::detail {

const std::vector<fs::path>& UserParameters::get_paths() const {
  return paths_;
}

const std::vector<std::string>& UserParameters::get_exclude_globs() const {
  return exclude_globs_;
}

size_t UserParameters::get_inactive_days_count() const {
  return inactive_days_count_;
}

void UserParameters::add_path(fs::path&& path) {
  paths_.emplace_back(std::move(path));
}

void UserParameters::add_exclude_glob(std::string&& glob) {
  exclude_globs_.emplace_back(std::move(glob));
}

void UserParameters::set_inactive_hours_count(size_t inactive_hours_count) {
  inactive_days_count_ = inactive_hours_count;
}

template <std::size_t N>
bool matches_any_glob(const std::string& path,
                      const std::array<std::string_view, N>& globs) {
  for (const auto& pattern : globs) {
    if (fnmatch(pattern.data(), path.c_str(), FNM_PATHNAME) == 0) {
      return true;
    }
  }
  return false;
}

std::chrono::system_clock::time_point last_time(
    std::chrono::system_clock::time_point t1,
    std::chrono::system_clock::time_point t2,
    std::chrono::system_clock::time_point t3) {
  return std::max({t1, t2, t3});
}

std::chrono::system_clock::time_point get_last_access_approx(
    const struct stat& info) {
  auto atime = std::chrono::system_clock::from_time_t(info.st_atime);
  auto mtime = std::chrono::system_clock::from_time_t(info.st_mtime);
  auto ctime = std::chrono::system_clock::from_time_t(info.st_ctime);

  return last_time(atime, mtime, ctime);
}

}  // namespace dcleaner::detail
