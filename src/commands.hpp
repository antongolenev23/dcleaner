#include <expected>
#include <vector>
#include <iostream>
#include "filesystem.hpp"
#include "commands_output.hpp"
#include "logger.hpp"

namespace dcleaner {

struct ExitSignal {};

using path = ghc::filesystem::path;
using ExecuteResult = std::expected<CommandOutput, ExitSignal>;


class Command {
public:
    Command(const detail::Logger&);
    virtual ~Command() = default;

    virtual ExecuteResult execute() = 0;
private:
    const detail::Logger& logger;
};


class Analyze : public Command {
public:
    Analyze(const detail::Logger&, detail::UserParameters&&);
    ExecuteResult execute() override;

private:
    detail::UserParameters parameters_;
};


class Delete : public Command {
public:
    Delete(const detail::Logger&, detail::UserParameters&&);
    ExecuteResult execute() override;

private:
    detail::UserParameters parameters_;
};


class Help : public Command {
public:
    ExecuteResult execute() override;
};

class Exit : public Command {
public:
    ExecuteResult execute() override;
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

