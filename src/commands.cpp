#include "commands.hpp"
#include <sys/stat.h>
#include <chrono>

namespace dcleaner {

Command::Command(detail::Logger& logger)
    : logger_(logger) 
{}
    
Analyze::Analyze(detail::Logger& logger, detail::UserParameters&& parameters)
    : Command{logger}
    , parameters_(parameters)
{}

ExecuteResult Analyze::execute() const {
    AnalyzeOutput output;

    CategorySummary inactive_summary{FileCategory::INACTIVE};
    CategorySummary empty_summary{FileCategory::EMPTY};
    CategorySummary cache_summary{FileCategory::CACHE};
    CategorySummary temp_summary{FileCategory::TEMPORARY};

    for(const fs::path& root : parameters_.get_paths()) {
        for(const auto& entry : fs::recursive_directory_iterator(root)) {
            // todo
        }
    }
}

Delete::Delete(detail::Logger& logger, detail::UserParameters&& parameters)
    : Command{logger}
    , parameters_(parameters)
{}

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

namespace detail {

// todo

void UserParameters::add_path(fs::path&& path) {
    paths_.emplace_back(std::move(path));  
}

void UserParameters::add_exclude_glob(std::string&& glob) {
    exclude_globs_.emplace_back(std::move(glob));
}

void UserParameters::set_inactive_hours_count(size_t inactive_hours_count) {
    inactive_hours_count_ = inactive_hours_count;
}

void UserParameters::set_min_age_hours_count(size_t min_age_hours_count) {
    min_age_hours_count_ = min_age_hours_count;
}

template <Flag T>
void UserParameters::set_flag(T flag) {
    flags_ |= static_cast<int>(flag);
}

} // namespace detail

} // namespace dcleaner
