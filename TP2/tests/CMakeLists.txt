cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB TESTS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/unit/src/*.c)  # Archivos de prueba

set(SRC_FILES ${CMAKE_SOURCE_DIR}/src/command.c
                ${CMAKE_SOURCE_DIR}/src/intern_command.c
                ${CMAKE_SOURCE_DIR}/src/shell.c
                ${CMAKE_SOURCE_DIR}/src/jobs.c
                ${CMAKE_SOURCE_DIR}/src/signals.c
                ${CMAKE_SOURCE_DIR}/src/process.c
)
                
# Mensaje para depuración
message("Archivos de prueba: ${TESTS_FILES}")
message("Archivos fuente: ${SRC_FILES}")

find_package(cJSON REQUIRED)
find_package(unity REQUIRED)

if(RUN_COVERAGE)
    message("Coverage enabled")
    find_program(LCOV_PATH lcov)
        if(NOT LCOV_PATH)
        message(FATAL_ERROR "lcov not found! Aborting...")
    endif()

    find_program(GCOV_PATH gcov)
        if(NOT GCOV_PATH)
        message(FATAL_ERROR "gcov not found! Aborting...")
    endif()

    find_program(GENHTML_PATH genhtml)
        if(NOT GENHTML_PATH)
        message(FATAL_ERROR "genhtml not found! Aborting...")
    endif()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage")
endif()

add_executable(test_${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/unit/src/test_main.c ${TESTS_FILES} ${SRC_FILES})

add_definitions(-DPROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")


target_compile_definitions(test_${PROJECT_NAME} PRIVATE RUN_TESTS)

target_include_directories(test_${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_include_directories(test_${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/unit/include)

target_link_libraries(test_${PROJECT_NAME} PRIVATE unity::unity cjson::cjson)

add_test(NAME test_${PROJECT_NAME} COMMAND test_${PROJECT_NAME})

