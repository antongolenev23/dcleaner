#include <map>
#include <string>
#include "files_category_info.hpp"

namespace dcleaner {
 
class CommandOutput {
    virtual ~CommandOutput() = default;
    virtual std::string to_string() const = 0;
};


class AnalyzeOutput : public CommandOutput {
public:
    std::string to_string() const override;

private:
    std::map<std::string, detail::FilesCategoryInfo> category_name_to_info_;
};


class DeleteOutput : public CommandOutput {
public:
    std::string to_string() const override;

private:
    std::map<std::string, detail::FilesCategoryInfo> category_name_to_info_;
};


class HelpOutput : public CommandOutput {
public:
    std::string to_string() const override;

private:
    std::string info_;
};

} // namespace dcleaner


