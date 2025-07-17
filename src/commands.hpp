#pragma once
#include <concepts>
#include <expected>
#include <memory>
#include <vector>
#include <iostream>
#include "filesystem.hpp"
#include "commands_output.hpp"
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

namespace detail {

template <typename T>
concept Flag = 
    std::same_as<T, FileCategory> || 
    std::same_as<T, DeletePolicy>;

class UserParameters {
public:
    UserParameters()
        : inactive_hours_count_(0)
        , min_age_hours_count_(0)
        , flags_(0)
    {}

    const std::vector<fs::path>& get_paths() const;
    void add_path(fs::path&& path);
    void add_exclude_glob(std::string&& glob);
    void set_inactive_hours_count(size_t inactive_hours_count);
    void set_min_age_hours_count(size_t min_age_hours_count);
    template <Flag T>
    void set_flag(T flag);

private:
    std::vector<fs::path> paths_;
    std::vector<std::string> exclude_globs_;
    size_t inactive_hours_count_;
    size_t min_age_hours_count_;
    // todo comment about flags
    int flags_;
};

} // namespace detail

}; // namespace dcleaner

