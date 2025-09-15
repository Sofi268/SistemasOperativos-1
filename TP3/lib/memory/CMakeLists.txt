cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(
  "MemoryLib"
  VERSION 1.0.0
  DESCRIPTION "Memory allocator library"
  LANGUAGES C
)

set(CMAKE_C_STANDARD 17)

# Directorios de include
include_directories(${CMAKE_SOURCE_DIR}/include)

# Archivos fuente
file(GLOB_RECURSE SOURCES "src/*.c")

# Opciones de cobertura
option(RUN_COVERAGE "Enable code coverage flags" OFF)
if(RUN_COVERAGE)
    message("Coverage enabled")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
endif()

# Crear librería compartida
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# -------------------------
# Test target con Unity
# -------------------------
file(GLOB_RECURSE TEST_SOURCES "tests/unit/src/*.c")

if(TEST_SOURCES)
    add_executable(test_${PROJECT_NAME} ${TEST_SOURCES})
    
    target_link_libraries(test_${PROJECT_NAME}
        PRIVATE
            ${PROJECT_NAME} 
            unity
    )

    target_include_directories(test_${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    
    add_definitions(-DPROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")
    target_compile_definitions(test_${PROJECT_NAME} PUBLIC RUN_TESTS)
    
    enable_testing()
    add_test(NAME test_${PROJECT_NAME} COMMAND test_${PROJECT_NAME})
endif()
