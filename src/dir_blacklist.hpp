#pragma once
#include <array>
#include <string_view>

namespace dcleaner::detail {

constexpr std::array<std::string_view, 39> BLACKLIST = {
    // Системные каталоги
    "/bin/**", "/boot/**", "/dev/**", "/etc/**", "/lib/**", "/lib64/**", "/proc/**", "/root/**", "/sbin/**", "/sys/**",
    "/usr/**", "/opt/**", "/snap/**", "/lost+found/**",

    // /var
    "/var/lib/**", "/var/log/**", "/var/mail/**", "/var/run/**", "/var/cache/**",

    // Домашние каталоги
    "/home/**/.ssh/**", "/home/**/.gnupg/**", "/home/**/.config/**", "/home/**/.local/**", "/home/**/.mozilla/**",
    "/home/**/.thunderbird/**", "/home/**/.dbus/**", "/home/**/.gnome/**", "/home/**/.kde/**", "/home/**/.cache/**",
    "/home/**/.npm/**", "/home/**/.docker/**", "/home/**/.password-store/**",

    // Конфиги (без **, если файлы)
    "/home/*/.bashrc", "/home/*/.profile", "/home/*/.bash_profile", "/home/*/.zshrc", "/home/*/.vimrc",
    "/home/*/.gitconfig", "/home/*/.kube/config"};

}  // namespace dcleaner::detail
