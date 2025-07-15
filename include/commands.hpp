#include <map>
#include <memory>
#include <vector>
#include "filesystem.hpp"
#include "commands_result.hpp"
#include "logger.hpp"

namespace dcleaner {

struct UserParameters {
    std::vector<std::string> exclude_globs_;
    size_t inactive_hours_count_;
    size_t min_age_hours_count_;
    uint8_t flags_;
};


class Command {
public:
    virtual ~Command() = default;

    virtual CommandResult execute() = 0;
};


class Analyze : public Command {
public:
    CommandResult execute() override;

private:
    UserParameters parameters_;
    std::shared_ptr<detail::Logger> logger_;
};


class Delete : public Command {
public:
    CommandResult execute() override;

private:
    UserParameters parameters_;
    std::shared_ptr<detail::Logger> logger_;
};


class Help : public Command {
public:
    CommandResult execute() override;
};

}; // namespace dcleaner

