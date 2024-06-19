# Script to bundle wxWidget libraries

set(APP_DIR "${CMAKE_BINARY_DIR}/GoOdf.app")

if(EXISTS "${APP_DIR}")
  execute_process(
    COMMAND dylibbundler
      --bundle-deps
      --overwrite-files
      --fix-file "${APP_DIR}/Contents/MacOS/GoOdf"
      --dest-dir "${APP_DIR}/Contents/Resources/"
      --install-path "@executable_path/../Resources/"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error_output
  )

  if(NOT ${result} EQUAL 0)
    message(FATAL_ERROR "dylibbundler failed: ${error_output}")
  else()
    message(STATUS "dylibbundler output: ${output}")
  endif()
else()
  message(FATAL_ERROR "Application bundle not found: ${APP_DIR}")
endif()