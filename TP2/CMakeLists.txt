cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(
  "MyShell"
  VERSION 1.0.0
  DESCRIPTION "Bourne Shell type"
  LANGUAGES C
)

include(CTest)
enable_testing()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "-g3 -Wall -pedantic -Wextra -Wconversion")

include_directories(include)

find_package(cJSON REQUIRED)
find_package(unity REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE cjson::cjson)

if(RUN_TESTS EQUAL 1 OR RUN_COVERAGE EQUAL 1)
  add_subdirectory(tests)
endif()
