#include "commands.hpp"
#include <iostream>

using namespace dcleaner;

int main() {
    Logger logger;
    detail::UserParameters params;
    params.add_path(fs::path("/home/anton/University/Dev"));
    params.set_flag(FileCategory::INACTIVE);
    params.set_flag(FileCategory::EMPTY);
    params.set_inactive_days_count(30);

    Analyze analyze(logger, std::move(params));
    ExecuteResult result = analyze.execute();
    if(result.has_value()) {
        for(const auto& err : (*result)->get_errors()) {
            std::cout << "path: " << err.path_str_ << ' ' << err.error_info_ << std::endl;
        }

        for(const auto& [key, value] : (*result)->get_summaries()) {
            std::string str_key = [&key]() {
                switch(key) {
                case FileCategory::INACTIVE:
                    return "Inactive";               
                case FileCategory::EMPTY:
                    return "Empty";
                }
                return "Unknown";
            }();

            std::cout << str_key << ": " << value.entries_count_ << " files, " << value.entries_size_in_bytes_ << " bytes\n";
        }
    }
}