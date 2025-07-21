#pragma once
#include <array>
#include <string_view>

namespace dcleaner::detail {

constexpr std::array<std::string_view, 14> CACHE_WHITELIST = {
    // Системные кеши и временные папки (которые безопасно очищать)
    "/tmp", "/var/tmp", "/var/cache", "/run/user/*",

    // Кеши приложений
    "/home/*/.cache",
    "/home/*/.local/share/Trash",  // корзина

    // Кеши популярных приложений
    "/home/*/.cache/google-chrome", "/home/*/.cache/chromium",
    "/home/*/.cache/firefox", "/home/*/.cache/vlc", "/home/*/.cache/spotify",

    // Кеши пакетных менеджеров
    "/var/cache/apt", "/var/cache/dnf", "/var/cache/pacman"};

}  // namespace dcleaner::detail
