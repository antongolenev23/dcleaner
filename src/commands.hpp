#pragma once
#include <chrono>
#include <concepts>
#include <expected>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

#include "../external/filesystem.hpp"
#include "commands_output.hpp"
#include "logger.hpp"

namespace dcleaner {

namespace fs = ghc::filesystem;

const fs::path DELETION_LIST_DIR = "deletion_list";
const fs::path DELETION_LIST_FILE = DELETION_LIST_DIR / "entries_to_delete.txt";

enum class Signal { EXIT, HELP, FILE_OPEN_ERROR };

template <typename T>
concept Flag = std::same_as<T, FileCategory> || std::same_as<T, DeletePolicy>;

namespace detail {

class UserParameters {
 public:
  UserParameters() : inactive_days_count_(30), flags_(0) {}

  template <Flag T>
  bool has_flag(T flag) const {
    return flags_ & static_cast<int>(flag);
  }

  const std::vector<fs::path>& get_paths() const;
  const std::vector<std::string>& get_exclude_globs() const;
  size_t get_inactive_days_count() const;
  int get_flags_() const;
  void add_path(fs::path&& path);
  void add_exclude_glob(std::string&& glob);
  void set_inactive_days_count(size_t inactive_days_count);

  template <Flag T>
  void set_flag(T flag) {
    flags_ |= static_cast<int>(flag);
  }

  template <Flag T>
  void remove_flag(T flag) {
    flags_ &= ~(static_cast<int>(flag));
  }

 private:
  std::vector<fs::path> paths_;
  std::vector<std::string> exclude_globs_;
  size_t inactive_days_count_;
  // todo comment about flags
  int flags_;
};

std::chrono::system_clock::time_point last_time(std::chrono::system_clock::time_point t1,
                                                std::chrono::system_clock::time_point t2,
                                                std::chrono::system_clock::time_point t3);

std::chrono::system_clock::time_point get_last_access_approx(const struct stat& info);

}  // namespace detail

using ExecuteResult = std::expected<std::unique_ptr<CommandOutput>, Signal>;

class Command {
 public:
  Command(Logger&);
  virtual ~Command() = default;

  virtual ExecuteResult execute() = 0;

 protected:
  Logger& logger_;
};

class Analyze : public Command {
 public:
  Analyze(Logger&, const detail::UserParameters&);
  ExecuteResult execute() override;

 private:
  void analyze_root_path(const fs::path& root, const fs::file_status& status, dcleaner::AnalyzeOutput& output);
  void check_activity(const std::string& path, const struct stat& info, AnalyzeOutput& output);
  void check_dir_is_empty(const ghc::filesystem::path& path, const struct stat& info, AnalyzeOutput& output);
  void recursive_directory_analyze(const fs::path& root_path, AnalyzeOutput& output);
  void analyze_file(const fs::path& path, AnalyzeOutput& output);
  void add_file_to_deletion_list(const fs::path& file);

  detail::UserParameters parameters_;
  std::ofstream deletion_list_file_;
};

class Delete : public Command {
 public:
  Delete(Logger&, const detail::UserParameters&);
  ExecuteResult execute() override;

  template <Flag T>
  bool has_flag(T flag) const {
    return flags_ & static_cast<int>(flag);
  }

  template <Flag T>
  void set_flag(T flag) {
    flags_ |= static_cast<int>(flag);
  }

  template <Flag T>
  void remove_flag(T flag) {
    flags_ &= ~(static_cast<int>(flag));
  }

 private:
  void delete_permanently(DeleteOutput& output);
  void delete_to_trash(DeleteOutput& output);

  std::ifstream deletion_list_file_;
  int flags_;
};

class Help : public Command {
 public:
  ExecuteResult execute() override;
};

class Exit : public Command {
 public:
  ExecuteResult execute() override;
};

};  // namespace dcleaner