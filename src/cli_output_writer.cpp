#include "cli_output_writer.hpp"
#include "help_text.hpp"

#include <iomanip>
#include <sstream>

namespace dcleaner {

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

using namespace detail::ansi;

void CLIOutputWriter::write_output(const ExecuteResult& result, std::ostream& out) {
  if (result) {
    auto command_output = result.value().get();

    if (AnalyzeOutput* analyze_output_ptr = dynamic_cast<AnalyzeOutput*>(command_output); analyze_output_ptr) {
      out << cyan << "=================== " << "Analyze" << " ==================\n\n" << reset;

      if (analyze_output_ptr->get_summaries().empty() && analyze_output_ptr->get_errors().empty()) {
        out << "No empty directories or inactive files found\n";
      }

      out << yellow;

      for (const auto& error : command_output->get_errors()) {
        out << error.path_str_ << " : " << error.error_info_ << '\n';
      }
      out << reset;

      for (const auto& [category, summary] : command_output->get_summaries()) {
        switch (category) {
          case FileCategory::INACTIVE:
            out << "Inactive: " << summary.entries_count_ << " files  " << green
                << detail::format_size(summary.entries_size_in_bytes_) << '\n';
            out << reset;
            break;
          case FileCategory::EMPTY:
            out << "Empty: " << summary.entries_count_ << " folders\n";
            break;
        }
      }
      out << '\n';
      out << cyan;
      out << "==============================================\n\n";
      out << reset;
      out.flush();

    } else if (DeleteOutput* delete_output_ptr = dynamic_cast<DeleteOutput*>(command_output); delete_output_ptr) {
      out << cyan << "=================== " << "Delete" << " ===================\n\n" << reset;

      if (delete_output_ptr->get_summaries().empty() && delete_output_ptr->get_errors().empty()) {
        out << "No empty directories or inactive files deleted\n";
      }

      out << yellow;

      for (const auto& error : command_output->get_errors()) {
        out << error.path_str_ << " : " << error.error_info_ << '\n';
      }

      out << reset;

      for (const auto& [category, summary] : command_output->get_summaries()) {
        switch (category) {
          case FileCategory::INACTIVE:
            out << "Inactive: " << summary.entries_count_ << " files  " << red
                << detail::format_size(summary.entries_size_in_bytes_) << '\n';
            out << reset;
            break;
          case FileCategory::EMPTY:
            out << "Empty: " << summary.entries_count_ << " folders\n";
            break;
        }
      }

      out << '\n';
      out << cyan;
      out << "==============================================\n\n";
      out << reset;
      out.flush();

    } else {
      out << red << "Error with execute result parsing occured\n" << reset;
      out.flush();
    }

  } else if(result.error() == Signal::FILE_OPEN_ERROR){
    out << red << "Error with deletion list file occured\n" << reset;
    out.flush();

  } else if(result.error() == Signal::NOT_ENOUGH_PARAMETERS) {
    out << yellow << "Not enough parameters. Write \"help\" for info.\n" << reset;
    out.flush();
  } else {
    out << HELP_TEXT;
    out.flush();
  } 
}

}  // namespace dcleaner
