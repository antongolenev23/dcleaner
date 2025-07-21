#pragma once
#include <array>
#include <string_view>

namespace dcleaner::detail {

constexpr std::array<std::string_view, 29> BLACKLIST = {
    // Системные критичные каталоги
    "/bin", "/boot", "/dev", "/etc", "/lib", "/lib64", "/proc", "/root",
    "/sbin", "/sys", "/usr",

    // Важные каталоги внутри /var
    "/var/lib", "/var/log", "/var/mail",

    // Домашние конфигурации пользователя
    "/home/*/.ssh", "/home/*/.gnupg", "/home/*/.config", "/home/*/.local/share",
    "/home/*/.mozilla", "/home/*/.thunderbird", "/home/*/.dbus",
    "/home/*/.gnome", "/home/*/.kde",

    // Важные конфигурационные файлы в домашней директории
    "/home/*/.bashrc", "/home/*/.profile", "/home/*/.bash_profile",
    "/home/*/.zshrc", "/home/*/.vimrc", "/home/*/.gitconfig"};

}  // namespace dcleaner::detail
