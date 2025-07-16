#pragma once
#include <expected>
#include <memory>
#include <vector>
#include <iostream>
#include "filesystem.hpp"
#include "commands_output.hpp"
#include "logger.hpp"

namespace dcleaner {

struct ExitSignal {};


using path = ghc::filesystem::path;
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

struct UserParameters {
    std::vector<path> paths_;
    std::vector<std::string> exclude_globs_;
    size_t inactive_hours_count_;
    size_t min_age_hours_count_;
    uint8_t flags_;
};

} // namespace detail

}; // namespace dcleaner

