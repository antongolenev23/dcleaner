#include <map>
#include <string>
#include "files_category_info.hpp"

namespace dcleaner {
 
struct CommandResult {
    virtual ~CommandResult() = default;
};


struct AnalyzeResult : CommandResult {
    std::map<std::string, detail::FilesCategoryInfo> category_name_to_info_;
};


struct DeleteResult : public CommandResult {
    std::map<std::string, detail::FilesCategoryInfo> category_name_to_info_;
};


struct HelpResult : public CommandResult {
    std::string info_;
};

} // namespace dcleaner


