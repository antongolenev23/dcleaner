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

TEST(TestGlobs, TestExtendedPatternMatching) {
  // 1. Тест на вопросительный знак (?)
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("file?.txt", "file1.txt"));
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("file?.txt", "fileA.txt"));
  EXPECT_FALSE(dcleaner::detail::match_glob_pattern("file?.txt", "file10.txt"));  // ? соответствует ровно 1 символу
  EXPECT_FALSE(dcleaner::detail::match_glob_pattern("file?.txt", "file.txt"));    // ? требует ровно 1 символ

  // 2. Тест на квадратные скобки ([])
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("file[123].txt", "file1.txt"));
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("file[0-9].txt", "file5.txt"));
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("file[a-z].txt", "filex.txt"));
  EXPECT_FALSE(dcleaner::detail::match_glob_pattern("file[123].txt", "file4.txt"));
  EXPECT_FALSE(dcleaner::detail::match_glob_pattern("file[!0-9].txt", "file1.txt")); // Отрицание

  // 3. Тест на фигурные скобки ({})
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("file.{txt,md}", "file.txt"));
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("file.{txt,md}", "file.md"));
  EXPECT_FALSE(dcleaner::detail::match_glob_pattern("file.{txt,md}", "file.pdf"));
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("test_{a,b,c}.log", "test_b.log"));

  // 4. Комбинированные тесты
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("image_[0-9]?.png", "image_12.png"));
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("log/{error,warn}?.txt", "log/warn1.txt"));
  EXPECT_FALSE(dcleaner::detail::match_glob_pattern("data/[a-z]?.csv", "data/12.csv"));

  // 5. Тесты с путями
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("/home/*/file[0-9].txt", "/home/user/file5.txt"));
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("/var/**/log?.txt", "/var/logs/archive/log1.txt"));
  EXPECT_FALSE(dcleaner::detail::match_glob_pattern("/tmp/{a,b}/file", "/tmp/c/file"));

  // 6. Граничные случаи
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("?", "a"));
  EXPECT_FALSE(dcleaner::detail::match_glob_pattern("?", "ab"));
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("[a]", "a"));
  EXPECT_FALSE(dcleaner::detail::match_glob_pattern("[a]", "b"));
  EXPECT_TRUE(dcleaner::detail::match_glob_pattern("{a}", "a"));
  EXPECT_FALSE(dcleaner::detail::match_glob_pattern("{a}", "b"));
}

TEST(TestGlobs, TestMatchesAnyGlobWithExtendedPatterns) {
  // Добавляем тесты для matches_any_glob с расширенными шаблонами
  constexpr std::array<std::string_view, 4> extended_blacklist = {
    "/tmp/trash?.log",       // ? в имени файла
    "/var/log/[a-z]?.log",   // [] в имени файла
    "/home/*/.{bashrc,zshrc}", // {} с альтернативами
    "/opt/{bin,lib}/**"      // {} с **
  };

  // 1. Тесты с ?
  EXPECT_TRUE(dcleaner::detail::matches_any_glob("/tmp/trash1.log", extended_blacklist));
  EXPECT_FALSE(dcleaner::detail::matches_any_glob("/tmp/trash.log", extended_blacklist)); // Нужен ровно 1 символ после trash

  // 2. Тесты с []
  EXPECT_TRUE(dcleaner::detail::matches_any_glob("/var/log/x1.log", extended_blacklist));
  EXPECT_FALSE(dcleaner::detail::matches_any_glob("/var/log/1.log", extended_blacklist));

  // 3. Тесты с {}
  EXPECT_TRUE(dcleaner::detail::matches_any_glob("/home/user/.bashrc", extended_blacklist));
  EXPECT_TRUE(dcleaner::detail::matches_any_glob("/home/user/.zshrc", extended_blacklist));
  EXPECT_FALSE(dcleaner::detail::matches_any_glob("/home/user/.profile", extended_blacklist));

  // 4. Комбинированные тесты
  EXPECT_TRUE(dcleaner::detail::matches_any_glob("/opt/bin/subdir/file", extended_blacklist));
  EXPECT_TRUE(dcleaner::detail::matches_any_glob("/opt/lib/another/file", extended_blacklist));
  EXPECT_FALSE(dcleaner::detail::matches_any_glob("/opt/sbin/program", extended_blacklist));
}