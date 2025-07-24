#include "glob_match_check.hpp"
#include "dir_blacklist.hpp"
#include "gtest/gtest.h"

TEST(TestGlobs, TestMatchesAnyGlobFunction) {
  // 1. Точное совпадение
  EXPECT_TRUE(dcleaner::detail::matches_any_glob("/bin/ls", dcleaner::detail::BLACKLIST));
  EXPECT_TRUE(dcleaner::detail::matches_any_glob("/home/user/.bashrc", dcleaner::detail::BLACKLIST));

  // 2. Рекурсивный ** (любое количество подкаталогов)
  EXPECT_TRUE(dcleaner::detail::matches_any_glob("/bin/subdir/abrt", dcleaner::detail::BLACKLIST));  // /bin/**
  EXPECT_TRUE(dcleaner::detail::matches_any_glob("/home/user/.ssh/config", dcleaner::detail::BLACKLIST));  // /home/**/.ssh/**
  EXPECT_TRUE(dcleaner::detail::matches_any_glob("/var/log/syslog", dcleaner::detail::BLACKLIST));  // /var/log/**
  EXPECT_TRUE(dcleaner::detail::matches_any_glob("/opt/local/legal", dcleaner::detail::BLACKLIST));  

  // 3. Одиночный * (ровно один компонент пути)
  EXPECT_TRUE(dcleaner::detail::matches_any_glob("/home/anton/.bashrc", dcleaner::detail::BLACKLIST));  // /home/*/.bashrc
  EXPECT_FALSE(dcleaner::detail::matches_any_glob("/home/anton/subdir/.bashrc", dcleaner::detail::BLACKLIST));  // Не совпадает с /home/*/.bashrc

  // 4. Частичное совпадение (без **)
  EXPECT_FALSE(dcleaner::detail::matches_any_glob("/bin", dcleaner::detail::BLACKLIST));  // Нет слеша в конце
  EXPECT_FALSE(dcleaner::detail::matches_any_glob("/home/user/.ssh", dcleaner::detail::BLACKLIST));  // Нет файла в конце

  // 5. Несовпадающие пути
  EXPECT_FALSE(dcleaner::detail::matches_any_glob("/home/anton/University/Dev", dcleaner::detail::BLACKLIST));
  EXPECT_FALSE(dcleaner::detail::matches_any_glob("/tmp/abc", dcleaner::detail::BLACKLIST));

  // 6. Граничные случаи
  EXPECT_FALSE(dcleaner::detail::matches_any_glob("", dcleaner::detail::BLACKLIST));  // Пустая строка
  EXPECT_FALSE(dcleaner::detail::matches_any_glob("/", dcleaner::detail::BLACKLIST));  // Корень (не в чёрном списке)
  EXPECT_FALSE(dcleaner::detail::matches_any_glob("/home", dcleaner::detail::BLACKLIST));  // /home без подкаталогов
}

TEST(TestGlobs, TestSinglePatternMatching) {
  // 1. Простые звёздочки
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("/home/*/.bashrc", "/home/user/.bashrc"));
  EXPECT_FALSE(dcleaner::detail::match_glob_pattern("/home/*/.bashrc", "/home/user/subdir/.bashrc"));

  // 2. Рекурсивные **
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("/var/**", "/var/log/syslog"));
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("/home/**/.ssh/*", "/home/user/.ssh/config"));

  // 3. Fnmatch-совместимость (если используется fnmatch внутри)
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("*.txt", "file.txt"));
  EXPECT_FALSE(dcleaner::detail::match_glob_pattern("*.txt", "file.log"));

  // 4. Краевые случаи
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("", ""));  // Пустые строки
  EXPECT_FALSE(dcleaner::detail::match_glob_pattern("*", ""));  // Путь пустой, но паттерн — *
}