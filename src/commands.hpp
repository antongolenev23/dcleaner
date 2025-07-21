#pragma once
#include <chrono>
#include <concepts>
#include <expected>
#include <iostream>
#include <memory>
#include <vector>

#include "commands_output.hpp"
#include "filesystem.hpp"
#include "logger.hpp"

namespace dcleaner {

struct ExitSignal {};

namespace fs = ghc::filesystem;
using ExecuteResult = std::expected<std::unique_ptr<CommandOutput>, ExitSignal>;

class Command {
 public:
  Command(detail::Logger&);
  virtual ~Command() = default;

  virtual ExecuteResult execute() const = 0;

 protected:
  detail::Logger& logger_;
};

class Analyze : public Command {
 public:
  Analyze(detail::Logger&, detail::UserParameters&&);
  ExecuteResult execute() const override;

 private:
  void categorize_file(const std::string& path_str, const struct stat& info,
                       AnalyzeOutput& output) const;
  void categorize_folder(const ghc::filesystem::path& path,
                         const struct stat& info, AnalyzeOutput& output) const;

  detail::UserParameters parameters_;
};

class Delete : public Command {
 public:
  Delete(detail::Logger&, detail::UserParameters&&);
  ExecuteResult execute() const override;

 private:
  detail::UserParameters parameters_;
};

class Help : public Command {
 public:
  ExecuteResult execute() const override;
};

class Exit : public Command {
 public:
  ExecuteResult execute() const override;
};

};  // namespace dcleaner

namespace dcleaner::detail {

template <typename T>
concept Flag = std::same_as<T, FileCategory> || std::same_as<T, DeletePolicy>;

class UserParameters {
 public:
  UserParameters() : inactive_days_count_(0), flags_(0) {}

  template <Flag T>
  bool has_flag(T flag) const {
    return flags_ & static_cast<int>(flag);
  }

  const std::vector<fs::path>& get_paths() const;
  const std::vector<std::string>& get_exclude_globs() const;
  size_t get_inactive_days_count() const;
  void add_path(fs::path&& path);
  void add_exclude_glob(std::string&& glob);
  void set_inactive_hours_count(size_t inactive_hours_count);

  template <Flag T>
  void set_flag(T flag) {
    flags_ |= static_cast<int>(flag);
  }

 private:
  std::vector<fs::path> paths_;
  std::vector<std::string> exclude_globs_;
  size_t inactive_days_count_;
  // todo comment about flags
  int flags_;
};

template <std::size_t N>
bool matches_any_glob(const std::string& path,
                      const std::array<std::string_view, N>& globs);

std::chrono::system_clock::time_point last_time(
    std::chrono::system_clock::time_point t1,
    std::chrono::system_clock::time_point t2,
    std::chrono::system_clock::time_point t3);

std::chrono::system_clock::time_point get_last_access_approx(const fs::path& p);

}  // namespace dcleaner::detail
