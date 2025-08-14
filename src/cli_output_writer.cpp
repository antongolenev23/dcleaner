#include "cli_output_writer.hpp"

#include <iomanip>
#include <sstream>

namespace dcleaner {

void CLIOutputWriter::write_output(const ExecuteResult& result, std::ostream& out) {
  if (result) {
    auto command_output = result.value().get();

    if (AnalyzeOutput* analyze_output_ptr = dynamic_cast<AnalyzeOutput*>(command_output); analyze_output_ptr) {
      out << "=================== " << "Analyze" << " ==================\n\n";
      for(const auto& error : command_output->get_errors()) {
        out << error.path_str_ << " : " << error.error_info_ << '\n'; 
      }
    
      for(const auto& [category, summary] : command_output->get_summaries()) {
        switch(category) {
          case FileCategory::INACTIVE:
            out << "Inactive: " << summary.entries_count_ << " files  " << detail::format_size(summary.entries_size_in_bytes_) << '\n';
            break;
          case FileCategory::EMPTY:
            out << "Empty: " << summary.entries_count_ << " folders" << '\n';
            break;
        }
      }
      out << '\n';
      out << "==============================================\n\n";

    } else if(DeleteOutput* delete_output_ptr = dynamic_cast<DeleteOutput*>(command_output); delete_output_ptr) {
      out << "=================== " << "Delete" << " ===================\n\n";
      for(const auto& error : command_output->get_errors()) {
        out << error.path_str_ << " : " << error.error_info_ << '\n'; 
      }

      for(const auto& [category, summary] : command_output->get_summaries()) {
        switch(category) {
          case FileCategory::INACTIVE:
            out << "Inactive: " << summary.entries_count_ << " files  " << detail::format_size(summary.entries_size_in_bytes_) << '\n'; 
            break;
          case FileCategory::EMPTY:
            out << "Empty: " << summary.entries_count_ << " folders" << '\n';
            break;
        }
      }

      out << '\n';
      out << "==============================================\n\n";
    } else {
      out << "Error with execute result parsing occured\n";
    }
  }
}

namespace detail {

std::string format_size(uint64_t bytes) {
  const char* units[] = {"B", "KB", "MB", "GB", "TB", "PB"};
  size_t unit_index = 0;
  double size = static_cast<double>(bytes);

  while (size >= 1024 && unit_index < std::size(units) - 1) {
    size /= 1024;
    ++unit_index;
  }

  std::ostringstream oss;
  oss << std::fixed << std::setprecision(size < 10 ? 1 : 0) << size << ' ' << units[unit_index];
  return oss.str();
}

}  // namespace detail

}  // namespace dcleaner
