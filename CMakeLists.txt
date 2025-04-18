# GoOdf cmake build file
# Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.10)

# Make sure that the version file will be considered by CMake
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/version.txt")

# Read PROGRAM_VERSION from file
if(EXISTS "${CMAKE_SOURCE_DIR}/version.txt")
  file(READ "${CMAKE_SOURCE_DIR}/version.txt" PROGRAM_VERSION)
  string(STRIP "${PROGRAM_VERSION}" PROGRAM_VERSION)
else()
  message(FATAL_ERROR "File ${CMAKE_SOURCE_DIR}/version.txt not found")
endif()

# Set the project name and info
project(
  GoOdf
  VERSION "${PROGRAM_VERSION}"
  DESCRIPTION "GoOdf - Software for creating and editing GrandOrgue ODFs"
  LANGUAGES CXX C
)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set output locations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(RESOURCE_INSTALL_DIR "share/${PROJECT_NAME}")
set(RESOURCE_DIR "${CMAKE_BINARY_DIR}/${RESOURCE_INSTALL_DIR}")

# Get wxWidgets
if(CMAKE_CROSSCOMPILING AND WIN32)
  set(wxWidgets_USE_UNICODE ON)
  set(wxWidgets_USE_STATIC ON)
endif()
find_package(wxWidgets REQUIRED html net adv core base)

# Get ImageMagic for icon conversion later
if(CMAKE_CROSSCOMPILING AND WIN32)
  find_program(ImageMagick_convert_EXECUTABLE convert)
else()
  find_package(ImageMagick COMPONENTS convert)
endif()

# create definition file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/GOODFDef.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/GOODFDef.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

# embed GrandOrgue built-in images
include(${CMAKE_SOURCE_DIR}/scripts/CreateResources.cmake)
create_resources(${CMAKE_SOURCE_DIR}/resources/GoImages/ ${CMAKE_CURRENT_BINARY_DIR}/GoImages/GoImages.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/GoImages)

# include wxWidgets convenience file
include(${wxWidgets_USE_FILE})

# set some compile options
if(CMAKE_CROSSCOMPILING AND WIN32)
  add_compile_options(-static)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-g -Wall -Wextra -pedantic)
else()
  add_compile_options(-O3 -ffast-math)
endif()

# Copy/create icons and resources to the build
if(CMAKE_CROSSCOMPILING AND WIN32)
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/share/${CMAKE_PROJECT_NAME}.ico")
    message(STATUS "Creating ${CMAKE_PROJECT_NAME}.rc")
    configure_file(${CMAKE_SOURCE_DIR}/resources/${CMAKE_PROJECT_NAME}.rc.in ${CMAKE_BINARY_DIR}/share/${CMAKE_PROJECT_NAME}.rc)
    message(STATUS "Creating ${CMAKE_PROJECT_NAME}.ico")
    execute_process(COMMAND ${ImageMagick_convert_EXECUTABLE} -background none -resize 256x256 "${CMAKE_SOURCE_DIR}/resources/icons/${CMAKE_PROJECT_NAME}.svg" "${CMAKE_PROJECT_NAME}.ico" WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/share)
  endif()
endif()

if(NOT EXISTS "${CMAKE_BINARY_DIR}/share/icons/hicolor/${ICON_SIZE}x${ICON_SIZE}/apps/${CMAKE_PROJECT_NAME}.png")
  set(ICON_SIZES 16 24 32 48 64 128 256 512 1024)
  foreach(ICON_SIZE IN ITEMS ${ICON_SIZES})
    message(STATUS "Creating ${CMAKE_PROJECT_NAME}.png in size ${ICON_SIZE}x${ICON_SIZE}")
    execute_process(COMMAND mkdir -p share/icons/hicolor/${ICON_SIZE}x${ICON_SIZE}/apps WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    execute_process(COMMAND ${ImageMagick_convert_EXECUTABLE} -background none -resize ${ICON_SIZE}x${ICON_SIZE} "${CMAKE_SOURCE_DIR}/resources/icons/${CMAKE_PROJECT_NAME}.svg" "${CMAKE_PROJECT_NAME}.png" WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/share/icons/hicolor/${ICON_SIZE}x${ICON_SIZE}/apps)
  endforeach()
endif()

# Copy desktop file for linux
if(UNIX AND NOT APPLE)
  message(STATUS "Creating ${CMAKE_PROJECT_NAME}.desktop")
  configure_file(${CMAKE_SOURCE_DIR}/resources/${CMAKE_PROJECT_NAME}.desktop.in ${CMAKE_BINARY_DIR}/share/${CMAKE_PROJECT_NAME}/applications/${CMAKE_PROJECT_NAME}.desktop COPYONLY)
endif()

set(APP_SRC
  src/GOODF.cpp
  src/GOODFFrame.cpp
  src/Button.cpp
  src/Drawstop.cpp
  src/GoSwitch.cpp
  src/Organ.cpp
  src/OrganPanel.cpp
  src/Enclosure.cpp
  src/EnclosurePanel.cpp
  src/Tremulant.cpp
  src/TremulantPanel.cpp
  src/Windchestgroup.cpp
  src/WindchestgroupPanel.cpp
  src/SwitchPanel.cpp
  src/Loop.cpp
  src/Attack.cpp
  src/Release.cpp
  src/Pipe.cpp
  src/Rank.cpp
  src/WAVfileParser.cpp
  src/PipeDialog.cpp
  src/ReleaseDialog.cpp
  src/AttackDialog.cpp
  src/PipeBorrowingDialog.cpp
  src/PipeCopyOffsetDialog.cpp
  src/PipeLoadingDialog.cpp
  src/RankPanel.cpp
  src/RankReference.cpp
  src/Stop.cpp
  src/StopPanel.cpp
  src/Coupler.cpp
  src/Divisional.cpp
  src/Manual.cpp
  src/ManualPanel.cpp
  src/CouplerPanel.cpp
  src/DivisionalPanel.cpp
  src/DivisionalCoupler.cpp
  src/DivisionalCouplerPanel.cpp
  src/General.cpp
  src/GeneralPanel.cpp
  src/ReversiblePiston.cpp
  src/ReversiblePistonPanel.cpp
  src/GoColor.cpp
  src/GoFontSize.cpp
  src/GoPanelSize.cpp
  src/GoImage.cpp
  src/DisplayMetrics.cpp
  src/GoPanel.cpp
  src/GoImagePanel.cpp
  src/DisplayMetricsPanel.cpp
  src/GoPanelPanel.cpp
  src/GUIElements.cpp
  src/GUIButton.cpp
  src/GUIManual.cpp
  src/ManualKeyCopyDialog.cpp
  src/GUIStop.cpp
  src/GUICoupler.cpp
  src/GUIDivisional.cpp
  src/GUIEnclosure.cpp
  src/GUITremulant.cpp
  src/GUISwitch.cpp
  src/GUIReversiblePiston.cpp
  src/GUIDivisionalCoupler.cpp
  src/GUIGeneral.cpp
  src/GUILabel.cpp
  src/GUIButtonPanel.cpp
  src/GUIEnclosurePanel.cpp
  src/GUILabelPanel.cpp
  src/GUIManualPanel.cpp
  src/OrganFileParser.cpp
  src/CopyElementAttributesDialog.cpp
  src/GUIRepresentationDrawingPanel.cpp
  src/GUIPanelRepresentation.cpp
  src/CmbParser.cpp
  src/CmbDialog.cpp
  src/DefaultPathsDialog.cpp
  src/SampleFileInfoDialog.cpp
  src/DoubleEntryDialog.cpp
  src/StopRankImportDialog.cpp
)

# add the executable
if(CMAKE_CROSSCOMPILING AND WIN32)
  add_executable(${CMAKE_PROJECT_NAME}
    WIN32
    ${APP_SRC}
    ${CMAKE_BINARY_DIR}/share/${CMAKE_PROJECT_NAME}.rc
  )
else()
  add_executable(${CMAKE_PROJECT_NAME}
    ${APP_SRC}
  )
endif()

# set windows specific linker options
if(CMAKE_CROSSCOMPILING AND WIN32)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    -static
    -lwinmm
  )
endif()

# link with wxWidgets
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
  ${wxWidgets_LIBRARIES}
)

# Strip binary for release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  if(APPLE)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND strip ${CMAKE_PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  elseif(CMAKE_CROSSCOMPILING AND WIN32)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND strip --strip-all ${CMAKE_PROJECT_NAME}.exe WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  else()
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND strip --strip-all ${CMAKE_PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  endif()
endif()

# Zip help files to the build
if(NOT EXISTS "${RESOURCE_DIR}/help")
  file(MAKE_DIRECTORY ${RESOURCE_DIR}/help)
endif()
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND zip -r ${RESOURCE_DIR}/help/help.zip . WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/help)

# Installation rules
if (APPLE)
  install(DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.app DESTINATION .)
  # Explicitly set the executable permission
  install(PROGRAMS "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/MacOS/${CMAKE_PROJECT_NAME}"
    DESTINATION "${PROJECT_NAME}.app/Contents/MacOS"
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
else()
  install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
  install(DIRECTORY ${CMAKE_BINARY_DIR}/share/icons/ DESTINATION share/icons FILES_MATCHING PATTERN "*.png")
  install(FILES ${CMAKE_BINARY_DIR}/share/${CMAKE_PROJECT_NAME}/help/help.zip DESTINATION share/${CMAKE_PROJECT_NAME}/help)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt DESTINATION .)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/AUTHORS DESTINATION .)
endif()
if(UNIX AND NOT APPLE)
  install(FILES ${CMAKE_BINARY_DIR}/share/${CMAKE_PROJECT_NAME}/applications/${CMAKE_PROJECT_NAME}.desktop DESTINATION share/${CMAKE_PROJECT_NAME}/applications)
endif()

# Packaging
set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CMAKE_PROJECT_DESCRIPTION}")
set(
  CPACK_PACKAGE_DESCRIPTION
  "${CMAKE_PROJECT_NAME}"
)
set(CPACK_PACKAGE_VERSION_MAJOR "${GoOdf_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${GoOdf_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION ${GoOdf_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_PROJECT_NAME}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_CONTACT "larspalo@yahoo.se")
set(CPACK_PACKAGE_EXECUTABLES "${CMAKE_PROJECT_NAME}" "${CMAKE_PROJECT_NAME}")
set(CPACK_SOURCE_IGNORE_FILES ".gitignore" "/build/")

if (APPLE)
  # Configure and install Info.plist
  configure_file(${CMAKE_SOURCE_DIR}/resources/Info.plist.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Info.plist)
  INSTALL(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Info.plist DESTINATION "${PROJECT_NAME}.app/Contents/")

  # Create the macOS application bundle
  add_custom_target(
    macOSApplication
    ALL
    DEPENDS ${CMAKE_PROJECT_NAME} resources
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/MacOS"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/share"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_BINARY_DIR}/bin/${CMAKE_PROJECT_NAME}"
      "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/MacOS/${CMAKE_PROJECT_NAME}"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/resources/GoOdf.icns"
      "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources/GoOdf.icns"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_BINARY_DIR}/share"
      "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/share"
    COMMAND "${CMAKE_COMMAND}" -DAPP_DIR="${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app" -P "${CMAKE_SOURCE_DIR}/scripts/SignMacOSApp.cmake"
    COMMAND "${CMAKE_COMMAND}" -DAPP_DIR="${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app" -P "${CMAKE_SOURCE_DIR}/scripts/BundleWxWidgetLibraries.cmake"
  )

  # CPACK configuration
  set(CPACK_SYSTEM_NAME "macOS")
  set(CPACK_GENERATOR DragNDrop)
  set(CPACK_DMG_FORMAT "UDZO")
  set(CPACK_DMG_VOLUME_NAME "GoOdf")
  set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/resources/GoOdf.icns")

elseif(WIN32)
  set(CPACK_SYSTEM_NAME "windows")
  set(CPACK_GENERATOR ZIP)
  set(CMAKE_SYSTEM_PROCESSOR "x86_64")

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CPACK_SYSTEM_NAME "linux")
  set(CPACK_GENERATOR TGZ)
endif()

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}.${CMAKE_SYSTEM_PROCESSOR}")
include(CPack)

message(STATUS "

============================================================================
CMake Generator            : ${CMAKE_GENERATOR}
Project name               : ${CMAKE_PROJECT_NAME}
Project version            : ${GoOdf_VERSION}
Build type                 : ${CMAKE_BUILD_TYPE}
Install prefix             : ${CMAKE_INSTALL_PREFIX}
Output directory           : ${CMAKE_BINARY_DIR}
============================================================================
wxWidgets Version          : ${wxWidgets_VERSION}
wxWidgets Unicode?         : ${wxWidgets_USE_UNICODE}
wxWidgets Debug?           : ${wxWidgets_USE_DEBUG}
wxWidgets Static linking   : ${wxWidgets_USE_STATIC}
wxWidgets config binary    : ${wxWidgets_CONFIG_EXECUTABLE}
wxWidgets configuration    : ${wxWidgets_CONFIGURATION}
============================================================================

")
