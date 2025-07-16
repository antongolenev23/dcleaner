#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "summary.hpp"

namespace dcleaner {
 
using CategorySummaries = std::vector<detail::CategorySummary>;

class CommandOutput {
public:
    virtual ~CommandOutput() = default;
};

class AnalyzeOutput : public CommandOutput {
public:
    void add_summary(detail::CategorySummary&&);
    const CategorySummaries& get_summaries_cref() const;

private:
    CategorySummaries summaries_;
};


class DeleteOutput : public CommandOutput {
public:
    void add_summary(detail::CategorySummary&&);
    const CategorySummaries& get_summaries_cref() const;

private:
    CategorySummaries summaries_;
};


class HelpOutput : public CommandOutput {
public:
    HelpOutput(std::string&&);
    const std::string& get_info_cref() const;

private:
    std::string info_;
};

} // namespace dcleaner


