find_package(Gtest REQUIRED)

add_executable(test_dcleaner test_dcleaner.cpp)

target_link_libraries(test_dcleaner GTest::GTest GTest::Main cleaner)

add_test()