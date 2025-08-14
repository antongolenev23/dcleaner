#include "../mock_logger.hpp"
#include "../setup_mock_fs.hpp"
#include "commands.hpp"
#include "dir_blacklist.hpp"
#include "filesystem.hpp"
#include "gtest/gtest.h"

using namespace dcleaner;
namespace fs = ghc::filesystem;

class AnalyzeClassTest : public ::testing::Test {
 protected:
  void SetUp() override { setup_mock_fs(); }

  void TearDown() override {
    const fs::path mock_path = get_mock_fs_path();
    fs::remove_all(mock_path);
    fs::remove_all(DELETION_LIST_DIR);
  }
};

TEST_F(AnalyzeClassTest, TestExecuteFunctionCorrectData) {
  MockLogger mock_logger;
  detail::UserParameters params;
  params.add_path(get_mock_fs_path());
  params.set_flag(FileCategory::INACTIVE);
  params.set_flag(FileCategory::EMPTY);
  params.set_inactive_days_count(29);

  Analyze analyze{mock_logger, std::move(params)};
  ExecuteResult result = analyze.execute();

  ASSERT_TRUE(result) << "Execute failed";

  const auto& sum = (*result)->get_summaries();
  ASSERT_GT(sum.count(FileCategory::INACTIVE), 0) << "No INACTIVE entries found";
  ASSERT_GT(sum.count(FileCategory::EMPTY), 0) << "No EMPTY entries found";
  EXPECT_EQ(1, sum.at(FileCategory::INACTIVE).entries_count_);
  EXPECT_EQ(1, sum.at(FileCategory::EMPTY).entries_count_);
}

TEST_F(AnalyzeClassTest, TestExecuteFunctionEmptyFolder) {
  MockLogger mock_logger;
  detail::UserParameters params;
  params.add_path(get_project_root_path() / "tests" / "test_data" / "empty_folder");
  params.set_flag(FileCategory::INACTIVE);
  params.set_flag(FileCategory::EMPTY);
  params.set_inactive_days_count(30);

  Analyze analyze{mock_logger, std::move(params)};
  ExecuteResult result = analyze.execute();

  ASSERT_TRUE(result) << "Execute failed";

  const auto& sum = (*result)->get_summaries();
  ASSERT_FALSE(sum.contains(FileCategory::INACTIVE));
  ASSERT_FALSE(sum.contains(FileCategory::EMPTY));
}

TEST_F(AnalyzeClassTest, TestExecuteTwoFolders) {
  MockLogger mock_logger;
  detail::UserParameters params;
  params.add_path(get_mock_fs_path());
  params.add_path(get_project_root_path() / "tests" / "test_data"/ "mock_fs_empty");
  params.set_flag(FileCategory::INACTIVE);
  params.set_flag(FileCategory::EMPTY);
  params.set_inactive_days_count(10);

  Analyze analyze{mock_logger, std::move(params)};
  ExecuteResult result = analyze.execute();

  ASSERT_TRUE(result) << "Execute failed";

  const auto& sum = (*result)->get_summaries();
  ASSERT_GT(sum.count(FileCategory::INACTIVE), 0) << "No INACTIVE entries found";
  ASSERT_GT(sum.count(FileCategory::EMPTY), 0) << "No EMPTY entries found";
  EXPECT_EQ(1, sum.at(FileCategory::INACTIVE).entries_count_);
  EXPECT_EQ(2, sum.at(FileCategory::EMPTY).entries_count_);
}