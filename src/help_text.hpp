#pragma once

#include <string_view>

constexpr std::string_view HELP_TEXT = R"(Available commands:

  analyze <paths...> [options]
    Analyze specified directories or files.
    Searches for:
      • Files that have not been used for more than the specified number of days
        (default: 30 days if --inactive is set without --inactive-days).
      • Empty directories (if --empty flag is provided).
    Note:
      At least one of --empty or --inactive must be specified,
      otherwise no analysis will be performed.
    Required:
      <paths...>            One or more paths to analyze.
    Options:
      --empty                Search for empty directories.
      --inactive             Search for inactive files.
      --inactive-days=<N>    Set inactivity threshold in days (used with --inactive).
      --exclude=<patterns>   Comma-separated glob patterns to exclude.
                             Example: **/main.{go,py},temp/*
    Examples:
      analyze /home/user/project --empty
      analyze /home/user/photo /home/user/texts/ --inactive --inactive-days=90 --exclude=**/*.tmp

  delete <paths...> [options]
    Delete files or directories identified by a prior analyze command.
    You can also run delete directly without analyze, but then you must
    specify the target paths and desired search options.
    Note:
      At least one of --empty or --inactive must be specified
      when running delete without a prior analyze.
    Required (if not preceded by analyze):
      <paths...>            One or more paths to delete.
    Options:
      --empty                Same as in analyze: search for empty directories.
      --inactive             Same as in analyze: search for inactive files.
      --inactive-days=<N>    Same as in analyze.
      --exclude=<patterns>   Same as in analyze.
      --force                Permanently delete without moving to trash.
    Examples:
      delete --force
      delete /home/user/old_logs /home/user/photo --inactive --inactive-days=60

  help
    Show this help message.

  exit
    Exit the program.

Glob patterns:
  Globs are simplified matching patterns used for specifying file and folder names.
  Supported special symbols:
    *        Matches zero or more characters in a single path segment.
    **       Matches zero or more path segments recursively.
    ?        Matches exactly one character.
    [abc]    Matches one character from the set (a, b, or c).
    [a-z]    Matches one character in the range a–z.
    [!abc]   Matches any single character NOT in the set.
    {a,b,c}  Matches any of the listed alternatives.
  Examples:
    *.txt                   — matches all .txt files in the current directory
    **/*.log                — matches all .log files in all subdirectories
    /home/*/.bashrc         — matches .bashrc in any user’s home directory
    /var/**/log?.txt        — matches log1.txt, log2.txt, etc., in any subdirectory under /var
    /tmp/{a,b}/file         — matches /tmp/a/file or /tmp/b/file
)";
