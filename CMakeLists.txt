# В CMakeLists.txt
option(DCLEANER_ENABLE_DEBUG_LOGS "Enable debug logs" OFF)

if (DCLEANER_ENABLE_DEBUG_LOGS)
  add_compile_definitions(DCLEANER_LOG_LEVEL=dcleaner::detail::LogLevel::DEBUG)
endif()
