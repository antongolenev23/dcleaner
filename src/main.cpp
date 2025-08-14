#include <iostream>
#include "commands.hpp"
#include "cli_output_writer.hpp"

using namespace dcleaner;

int main() {
  Logger logger;
  detail::UserParameters params;
  params.add_path(
      fs::path("/home/anton/Документы/test_dir"));
  params.set_flag(FileCategory::INACTIVE);
  params.set_flag(FileCategory::EMPTY);
//   params.set_flag(DeletePolicy::FORCE);
  params.set_inactive_days_count(0);
  params.add_exclude_glob("**/main.{go,py}");

  Analyze analyze(logger, params);
  ExecuteResult analyze_result = analyze.execute();
  CLIOutputWriter::write_output(analyze_result, std::cout);

  Delete del{logger, params};
  ExecuteResult delete_result = del.execute();
  CLIOutputWriter::write_output(delete_result, std::cout);
}