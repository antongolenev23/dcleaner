#include "commands_output.hpp"
#include <sstream>
#include <iomanip>

namespace dcleaner {

    
void AnalyzeOutput::add_summary(CategorySummary&& summary) {
    summaries_.push_back(std::move(summary));
}

const CategorySummaries& AnalyzeOutput::get_summaries() const {
    return summaries_;
}

void DeleteOutput::add_summary(CategorySummary&& summary) {
    summaries_.push_back(std::move(summary));
}

const CategorySummaries& DeleteOutput::get_summaries() const {
    return summaries_;
}

HelpOutput::HelpOutput(std::string&& help_info) 
    : info_(std::move(help_info))
{}

const std::string& HelpOutput::get_info() const {
    return info_;
}

} // namespace dcleaner