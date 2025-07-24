#pragma once
#include "filesystem.hpp"
#include <unistd.h> // для fsync, sync
#include <fcntl.h> // для AT_* констант
#include <sys/stat.h> // для utimensat
#include <thread>

inline ghc::filesystem::path get_project_root_path() {
    // Получаем путь к проекту (предполагаем, что файл находится в dcleaner/tests/)
    static const ghc::filesystem::path base_path = []() {
        return ghc::filesystem::canonical(__FILE__).parent_path().parent_path();
    }();
    return base_path;
}

inline ghc::filesystem::path get_mock_fs_path() {
    return get_project_root_path() / "tests" / "test_data" / "mock_fs";
}

inline void setup_mock_fs() {
    auto mock_path = get_mock_fs_path();
    
    if (ghc::filesystem::exists(mock_path)) {
        ghc::filesystem::remove_all(mock_path);
        sync();
    }
    
    ghc::filesystem::create_directories(mock_path / "empty_dir");
    
    const int days_old_count = 30;
    auto old_time = ghc::filesystem::file_time_type::clock::now() - std::chrono::days(days_old_count);
    
    // Старый файл
    auto old_file = mock_path / "old_file.txt";
    std::ofstream(old_file) << "test";
    
    // Преобразуем время в секунды для timespec
    auto old_time_sec = std::chrono::duration_cast<std::chrono::seconds>(old_time.time_since_epoch()).count();
    struct timespec times[2] = {
        {old_time_sec, 0},  // atime
        {old_time_sec, 0}   // mtime
    };
    utimensat(AT_FDCWD, old_file.c_str(), times, AT_SYMLINK_NOFOLLOW);
    
    // Новый файл
    std::ofstream(mock_path / "recent_file.txt") << "test";
}
