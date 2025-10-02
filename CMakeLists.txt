cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME GameSaveSyncClient)

set(QT_MAJOR_VERSION 6)

project(${TARGET_NAME} VERSION 0.0.1 DESCRIPTION "GameSaveSyncClient")

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#See if the environment var is set
if(DEFINED ENV{Qt${QT_MAJOR_VERSION}_HOME})
    message(STATUS "Looking for Qt in: " $ENV{Qt${QT_MAJOR_VERSION}_HOME})
else()
    message(STATUS "Qt${QT_MAJOR_VERSION}_HOME environment variable not set. Checking default paths.")
endif()

#find_package should find everything fine so long as the ENV Variable is set or, for linux systems,
#it is in the default install path.
find_package(Qt${QT_MAJOR_VERSION} COMPONENTS Widgets REQUIRED PATHS $ENV{Qt${QT_MAJOR_VERSION}_HOME})

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release"
      CACHE STRING "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)"
      FORCE
  )
endif()

add_executable(${TARGET_NAME}
    include/window.hpp
    src/window.cpp
    src/main.cpp)

target_compile_definitions(${TARGET_NAME} PRIVATE
  $<$<CONFIG:Debug>:DEBUG>
  $<$<CONFIG:Release>:NDEBUG>
)

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CONFIG:Debug>:/Zi /Od>
    $<$<CONFIG:Release>:/O2>
    /W4 /WX
  )
else()
    target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:Release>:-O3>
    -Wall -Wextra -pedantic -Werror
  )
endif()

target_include_directories(${TARGET_NAME} PRIVATE ${Qt${QT_MAJOR_VERSION}_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

set_target_properties(${TARGET_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/Debug"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
)

#Add any extra libs to link also.
target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_MAJOR_VERSION}::Widgets)
