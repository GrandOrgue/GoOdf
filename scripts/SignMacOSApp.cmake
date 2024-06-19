# Script for macOS code signing

set(APP_DIR "${CMAKE_BINARY_DIR}/GoOdf.app")

if(EXISTS "${APP_DIR}")
  execute_process(COMMAND codesign --force --sign - "${APP_DIR}")
  message("Checking code signature...")
  execute_process(COMMAND codesign --verify --deep "${APP_DIR}")
else()
  message(FATAL_ERROR "Application bundle not found: ${APP_DIR}")
endif()
