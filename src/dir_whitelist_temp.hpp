#pragma once
#include <array>
#include <string_view>

namespace dcleaner::detail {

constexpr std::array<std::string_view, 17> TEMP_FILES_WHITELIST = {
    // Общие системные временные каталоги
    "/tmp/*", "/var/tmp/*", "/run/user/*/temp/*",

    // Временные файлы в домашнем каталоге
    "/home/*/.cache/tmp/*", "/home/*/.cache/temp/*", "/home/*/tmp/*",
    "/home/*/temp/*",

    // Временные файлы браузеров (частично перекрывается с кешами)
    "/home/*/.mozilla/firefox/*/tmp/*",
    "/home/*/.mozilla/firefox/*/startupCache/*",
    "/home/*/.config/google-chrome/Default/ShaderCache/*",
    "/home/*/.config/chromium/Default/ShaderCache/*",

    // Временные файлы сборщиков пакетов (примеры)
    "/var/cache/apt/archives/partial/*", "/var/cache/dnf/tmp/*",
    "/var/cache/pacman/pkg/*",

    // Временные файлы редакторов и IDE
    "/home/*/.cache/vimtmp/*", "/home/*/.cache/emacs/*",
    "/home/*/.cache/code/*",  // VSCode
};

}  // namespace dcleaner::detail
