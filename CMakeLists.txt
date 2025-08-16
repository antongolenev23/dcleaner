cmake_minimum_required(VERSION 3.20)
project(Dcleaner)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")

# change to ON if you want debug
option(ENABLE_DEBUG "Build in Debug mode" OFF)
option(BUILD_TESTS "Build tests" OFF)

if(ENABLE_DEBUG)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
  add_compile_definitions(DCLEANER_LOG_LEVEL=dcleaner::LogLevel::DEBUG)

  # Включаем AddressSanitizer + UndefinedBehaviorSanitizer
  set(SANITIZER_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")
  set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${SANITIZER_FLAGS}")

  # Если есть clang-tidy — подключаем
  find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
  if(CLANG_TIDY_EXE)
      message(STATUS "clang-tidy найден: ${CLANG_TIDY_EXE}")
      set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
  endif()

else()
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()


add_executable(dcleaner "${SRC_PATH}/main.cpp")
install(TARGETS dcleaner DESTINATION bin)

add_library(dcleaner_code STATIC
  "${SRC_PATH}/glob_match_check.cpp"
  "${SRC_PATH}/logger.cpp"
  "${SRC_PATH}/commands.cpp"
  "${SRC_PATH}/commands_output.cpp"
  "${SRC_PATH}/cli_output_writer.cpp"
  "${SRC_PATH}/cli_input_parser.cpp"
)

target_include_directories(dcleaner_code PUBLIC 
  "${CMAKE_CURRENT_SOURCE_DIR}/external"
)

target_link_libraries(dcleaner PRIVATE dcleaner_code)

if(BUILD_TESTS) 
  enable_testing()
  add_subdirectory(tests)
endif()
