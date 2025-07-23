cmake_minimum_required(VERSION 3.11)
project(Dcleaner)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")

option(ENABLE_DEBUG "Build in Debug mode" ON)

if(ENABLE_DEBUG)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
  add_compile_definitions(DCLEANER_LOG_LEVEL=dcleaner::LogLevel::DEBUG)
else()
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

add_executable(dcleaner
  "${SRC_PATH}/main.cpp"
  "${SRC_PATH}/logger.cpp"
  "${SRC_PATH}/commands.cpp"
  "${SRC_PATH}/commands_output.cpp"
  "${SRC_PATH}/cli_output_writer.cpp"
)

target_include_directories(dcleaner PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external")

add_subdirectory(tests)
