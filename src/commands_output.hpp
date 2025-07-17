#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "summary.hpp"

namespace dcleaner {
 
using CategorySummaries = std::vector<CategorySummary>;

class CommandOutput {
public:
    virtual ~CommandOutput() = default;
};


class AnalyzeOutput : public CommandOutput {
public:
    void add_summary(CategorySummary&&);
    const CategorySummaries& get_summaries() const;

private:
    CategorySummaries summaries_;
};


class DeleteOutput : public CommandOutput {
public:
    void add_summary(CategorySummary&&);
    const CategorySummaries& get_summaries() const;

private:
    CategorySummaries summaries_;
};


class HelpOutput : public CommandOutput {
public:
    explicit HelpOutput(std::string&&);
    const std::string& get_info() const;

private:
    std::string info_;
};

} // namespace dcleaner


