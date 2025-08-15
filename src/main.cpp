#include <iostream>
#include "cli_input_parser.hpp"
#include "cli_output_writer.hpp"
#include "commands.hpp"

using namespace dcleaner;

int main(int argc, char* argv[]) {
  Logger logger;

  CLIInputParser parser{logger};
  CLIOutputWriter output_writer;

  while (true) {
    ParsingResult result = parser.parse_input(std::cin);
    if (result) {
      if (Analyze* analyze_ptr = dynamic_cast<Analyze*>(result.value().get()); analyze_ptr) {
        ExecuteResult output = analyze_ptr->execute();
        output_writer.write_output(output, std::cout);
      } else if (Delete* delete_ptr = dynamic_cast<Delete*>(result.value().get()); delete_ptr) {
        ExecuteResult output = delete_ptr->execute();
        output_writer.write_output(output, std::cout);
      }
    } else {
      Signal signal = result.error();
      if (signal == Signal::HELP) {
        Help help{logger};
        ExecuteResult output = help.execute();
        output_writer.write_output(output, std::cout);

      } else if (signal == Signal::EXIT) {
        Exit exit{logger};
        exit.execute();
        break;

      } else if (signal == Signal::NOT_ENOUGH_PARAMETERS) {
        std::cout << detail::ansi::yellow;
        std::cout << "Not enough parameters. Write \"help\" for info.\n";
        std::cout << detail::ansi::reset;
        std::cout.flush();

      } else if (signal == Signal::WRONG_PARAMETERS) {
        std::cout << detail::ansi::yellow;
        std::cout << "Wrong parameters. Write \"help\" for info.\n";
        std::cout << detail::ansi::reset;
        std::cout.flush();

      } else if (signal == Signal::UNKNOWN_COMMAND) {
        std::cout << detail::ansi::yellow;
        std::cout << "Unknown command. Write \"help\" for info.\n";
        std::cout << detail::ansi::reset;
        std::cout.flush();

      }
    }
  }
}