#include <iostream>
#include "cli_input_parser.hpp"
#include "cli_output_writer.hpp"
#include "commands.hpp"
#include "logger.hpp"

int main(int argc, char* argv[]) {
  dcleaner::fs::create_directories(dcleaner::LOGS_DIR);
  std::ofstream log_file(dcleaner::LOGS_FILE.native());
  if (!log_file.is_open()) {
    std::cout << "Error occured with logs file opening\n";
    return 1;
  }

  dcleaner::Logger logger{log_file};

  dcleaner::CLIInputParser parser{logger};
  dcleaner::CLIOutputWriter output_writer;

  while (true) {
    try {
      dcleaner::ParsingResult result = parser.parse_input(std::cin);

      if (result) {
        if (dcleaner::Analyze* analyze_ptr = dynamic_cast<dcleaner::Analyze*>(result.value().get()); analyze_ptr) {
          dcleaner::ExecuteResult output = analyze_ptr->execute();
          output_writer.write_output(output, std::cout);
        } else if (dcleaner::Delete* delete_ptr = dynamic_cast<dcleaner::Delete*>(result.value().get()); delete_ptr) {

          dcleaner::ExecuteResult output = delete_ptr->execute();
          output_writer.write_output(output, std::cout);
        }
      } else {
        dcleaner::Signal signal = result.error();
        if (signal == dcleaner::Signal::HELP) {
          dcleaner::Help help{logger};
          dcleaner::ExecuteResult output = help.execute();
          output_writer.write_output(output, std::cout);

        } else if (signal == dcleaner::Signal::EXIT) {
          dcleaner::Exit exit{logger};
          exit.execute();
          break;

        } else if (signal == dcleaner::Signal::NOT_ENOUGH_PARAMETERS) {
          std::cout << dcleaner::detail::ansi::yellow;
          std::cout << "Not enough parameters. Write \"help\" for info.\n";
          std::cout << dcleaner::detail::ansi::reset;
          std::cout.flush();

        } else if (signal == dcleaner::Signal::WRONG_PARAMETERS) {
          std::cout << dcleaner::detail::ansi::yellow;
          std::cout << "Wrong parameters. Write \"help\" for info.\n";
          std::cout << dcleaner::detail::ansi::reset;
          std::cout.flush();

        } else if (signal == dcleaner::Signal::UNKNOWN_COMMAND) {
          std::cout << dcleaner::detail::ansi::yellow;
          std::cout << "Unknown command. Write \"help\" for info.\n";
          std::cout << dcleaner::detail::ansi::reset;
          std::cout.flush();
        }
      }
    } catch (const std::runtime_error& e) {
      std::cout << dcleaner::detail::ansi::yellow;
      std::cout << "Not enough parameters. Write \"help\" for info.\n";
      std::cout << dcleaner::detail::ansi::reset;
      std::cout.flush();
    }
  }
}