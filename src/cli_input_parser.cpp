#include "cli_input_parser.hpp"
#include <memory>

namespace dcleaner {

std::vector<std::string> CLIInputParser::split_stream(std::istream& is) {
  std::vector<std::string> tokens;
  std::string line;

  if (!std::getline(is, line)) {
    return tokens;
  }

  std::istringstream iss(line);
  std::string token;
  while (iss >> token) {
    tokens.push_back(std::move(token));
  }

  return tokens;
}

ParsingResult CLIInputParser::parse_input(std::istream& input_stream) {
  auto tokens = split_stream(input_stream);
  if (tokens.empty())
    return std::unexpected{Signal::NOT_ENOUGH_PARAMETERS};

  const std::string& cmd_name = tokens[0];

  if (cmd_name == "analyze") {
    detail::UserParameters parameters;
    for (size_t i = 1; i < tokens.size(); ++i) {
      const std::string& t = tokens[i];
      if (t == "--empty")
        parameters.set_flag(FileCategory::EMPTY);
      else if (t == "--inactive")
        parameters.set_flag(FileCategory::INACTIVE);
      else if (t.rfind("--inactive-days=", 0) == 0) {
        try {
          parameters.set_inactive_days_count(std::stoi(t.substr(16)));
        } catch (...) {
          return std::unexpected{Signal::WRONG_PARAMETERS};
        }
      } else if (t.rfind("--exclude=", 0) == 0) {
        std::string patterns = t.substr(10);
        std::istringstream pstream(patterns);
        std::string pattern;
        while (std::getline(pstream, pattern, ',')) {
          parameters.add_exclude_glob(std::move(pattern));
        }
      } else {
        parameters.add_path(t);
      }
    }

    if(!parameters.has_flag(FileCategory::INACTIVE) && !parameters.has_flag(FileCategory::EMPTY)) {
      return std::unexpected{Signal::NOT_ENOUGH_PARAMETERS};
    }
    return std::make_unique<Analyze>(logger_, std::move(parameters));

  } else if (cmd_name == "delete") {
    detail::UserParameters parameters;

    for (size_t i = 1; i < tokens.size(); ++i) {

      const std::string& t = tokens[i];
      if (t == "--empty")
        parameters.set_flag(FileCategory::EMPTY);
      else if (t == "--inactive")
        parameters.set_flag(FileCategory::INACTIVE);
      else if (t.rfind("--inactive-days=", 0) == 0) {
        try {
          parameters.set_inactive_days_count(std::stoi(t.substr(16)));
        } catch (...) {
          return std::unexpected{Signal::WRONG_PARAMETERS};
        }
      } else if (t.rfind("--exclude=", 0) == 0) {
        std::string patterns = t.substr(10);
        std::istringstream pstream(patterns);
        std::string pattern;
        while (std::getline(pstream, pattern, ',')) {
          parameters.add_exclude_glob(std::move(pattern));
        }
      } else if (t == "--force")
        parameters.set_flag(DeletePolicy::FORCE);
      else {
        parameters.add_path(t);
      }
    }

    return std::make_unique<Delete>(logger_, std::move(parameters));

  } else if (cmd_name == "help") {
    return std::unexpected{Signal::HELP};
  } else if (cmd_name == "exit") {
    return std::unexpected{Signal::EXIT};
  }

  return std::unexpected{Signal::UNKNOWN_COMMAND};
}

}  // namespace dcleaner
