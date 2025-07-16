#include "commands.hpp"

namespace dcleaner {

Command::Command(detail::Logger& logger)
    : logger_(logger) 
{}
    
Analyze::Analyze(detail::Logger& logger, detail::UserParameters&& parameters)
    : Command{logger}
    , parameters_(parameters)
{}

ExecuteResult Analyze::execute() const {
    // todo
}

Delete::Delete(detail::Logger& logger, detail::UserParameters&& parameters)
    : Command{logger}
    , parameters_(parameters)
{}

ExecuteResult Delete::execute() const {
    // todo
}

ExecuteResult Help::execute() const {
    std::unique_ptr<CommandOutput> help_output = std::make_unique<HelpOutput>("dummy text");
    return help_output;
}

ExecuteResult Exit::execute() const {
    return std::unexpected(ExitSignal{});
}

} // namespace dcleaner
