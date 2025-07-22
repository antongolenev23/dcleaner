#include "commands.hpp"

#include <fnmatch.h>
#include <sys/stat.h>

#include <chrono>
#include <format>

#include "dir_blacklist.hpp"
#include "dir_whitelist_cache.hpp"
#include "dir_whitelist_temp.hpp"

using namespace dcleaner::detail;

namespace dcleaner {

Command::Command(Logger& logger) : logger_(logger) {}

Analyze::Analyze(Logger& logger, UserParameters&& parameters) : Command{logger}, parameters_(parameters) {}

ExecuteResult Analyze::execute() const {
  LOG_DEBUG(logger_, "call Analyze::execute()");
  AnalyzeOutput output;

  for (const fs::path& root : parameters_.get_paths()) {
    LOG_DEBUG(logger_, std::format("start analyze {}", root.native()));
    std::error_code ec;
    fs::file_status status = fs::symlink_status(root, ec);

    if (ec) {
      output.add_error(root.string(), ec.message());
      LOG_WARNING(logger_, std::format("can't get status {}: {}", root.native(), ec.message()));
      continue;
    }

    if (fs::is_directory(status)) {
      recursive_directory_analyze(root, output);
    } else if (fs::is_regular_file(status)) {
      analyze_file(root, output);
    } else {
      output.add_error(root.string(), "Unsupported file type");
      LOG_WARNING(logger_, std::format("{}: unsupported file type", root.native()));
    }
  }

  return std::make_unique<CommandOutput>(std::move(output));
}

void Analyze::categorize_file(const std::string& path_str, const struct stat& info, AnalyzeOutput& output) const {
  if (parameters_.has_flag(FileCategory::INACTIVE)) {
    auto last_access_time = get_last_access_approx(info);
    auto time_now = std::chrono::system_clock::now();

    auto duration = last_access_time < time_now ? time_now - last_access_time : last_access_time - time_now;

    auto days = duration_cast<std::chrono::days>(duration).count();
    size_t days_passed = static_cast<size_t>(days);

    if (days_passed >= parameters_.get_inactive_days_count()) {
      output.increment_summary(FileCategory::INACTIVE, 1, info.st_size);
      LOG_DEBUG(logger_, std::format("{}: added to inactive", path_str));
      return;
    }
  }
  if (parameters_.has_flag(FileCategory::CACHE)) {
    if (matches_any_glob(path_str, CACHE_WHITELIST)) {
      LOG_DEBUG(logger_, std::format("{}: added to cache", path_str));
      output.increment_summary(FileCategory::CACHE, 1, info.st_size);
      return;
    }
  }
  if (parameters_.has_flag(FileCategory::TEMPORARY)) {
    if (matches_any_glob(path_str, TEMP_FILES_WHITELIST)) {
      LOG_DEBUG(logger_, std::format("{}: added to temp", path_str));
      output.increment_summary(FileCategory::TEMPORARY, 1, info.st_size);
    }
  }
}

void Analyze::categorize_directory(const ghc::filesystem::path& path, const struct stat& info,
                                   AnalyzeOutput& output) const {
  if (parameters_.has_flag(FileCategory::EMPTY)) {
    std::error_code ec;
    bool is_empty = fs::is_empty(path, ec);
    if (!ec) {
      if (is_empty) {
        output.increment_summary(FileCategory::EMPTY, 1, info.st_size);
      }
    } else {
      LOG_WARNING(logger_, std::format("{}: {}", path.native(), ec.message()));
    }
  }
}

void Analyze::recursive_directory_analyze(const fs::path& root_path, AnalyzeOutput& output) const {
  std::error_code ec;

  for (fs::recursive_directory_iterator it(root_path, fs::directory_options::skip_permission_denied, ec), end;
       it != end; it.increment(ec)) {

    if (ec) {
      LOG_WARNING(logger_, std::format("{}: {}", (*it).path().native(), ec.message()));
      ec.clear();
      continue;
    }

    const fs::directory_entry& entry = *it;

    if (entry.is_symlink(ec)) {
      LOG_WARNING(logger_, std::format("{}: {}", entry.path().native(), ec.message()));
      ec.clear();
      continue;
    }

    const auto& path = entry.path();
    std::string path_str = path.string();

    if (matches_any_glob(path_str, BLACKLIST)) {
      LOG_INFO(logger_, std::format("{}: this path is in blacklist", path_str));
      continue;
    }

    struct stat info;
    if (stat(path_str.c_str(), &info) != 0) {
      LOG_WARNING(logger_, std::format("{}: error getting stat", path_str));
      continue;
    }

    if (S_ISREG(info.st_mode)) {
      categorize_file(path_str, info, output);
    } else if (S_ISDIR(info.st_mode)) {
      categorize_directory(path, info, output);
    }
  }
}

void Analyze::analyze_file(const fs::path& path, AnalyzeOutput& output) const {
  std::string path_str = path.string();

  struct stat info;
  if (stat(path_str.c_str(), &info) != 0) {
    LOG_WARNING(logger_, std::format("{}: error getting stat", path_str));
    return;
  }
  categorize_file(path_str, info, output);
}

Delete::Delete(Logger& logger, UserParameters&& parameters) : Command{logger}, parameters_(parameters) {}

ExecuteResult Delete::execute() const {
  // todo
}

ExecuteResult Help::execute() const {
  // todo help text
  std::unique_ptr<CommandOutput> help_output = std::make_unique<HelpOutput>("dummy text");
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
bool matches_any_glob(const std::string& path, const std::array<std::string_view, N>& globs) {
  for (const auto& pattern : globs) {
    if (fnmatch(pattern.data(), path.c_str(), FNM_PATHNAME) == 0) {
      return true;
    }
  }
  return false;
}

std::chrono::system_clock::time_point last_time(std::chrono::system_clock::time_point t1,
                                                std::chrono::system_clock::time_point t2,
                                                std::chrono::system_clock::time_point t3) {
  return std::max({t1, t2, t3});
}

std::chrono::system_clock::time_point get_last_access_approx(const struct stat& info) {
  auto atime = std::chrono::system_clock::from_time_t(info.st_atime);
  auto mtime = std::chrono::system_clock::from_time_t(info.st_mtime);
  auto ctime = std::chrono::system_clock::from_time_t(info.st_ctime);

  return last_time(atime, mtime, ctime);
}

}  // namespace dcleaner::detail
