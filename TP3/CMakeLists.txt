cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Activar CMP0048 para poder usar VERSION en project()
cmake_policy(SET CMP0048 NEW)

project(
    "Memory_Optimization"
    VERSION 1.0.0
    DESCRIPTION "Memory analysis and resources optimization"
    LANGUAGES C
)

include(CTest)
enable_testing()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "-g3 -Wall -pedantic -Wextra -Wconversion")

include_directories(lib/memory/include)

include(FetchContent)

set(UNITY_GIT_URL "https://github.com/ThrowTheSwitch/Unity.git")

# Enable debug
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
  Unity
  GIT_REPOSITORY ${UNITY_GIT_URL}
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/Unity
  UPDATE_COMMAND "" # Disable the update command
)

# ensures the named dependencies have been populated, either by an earlier call or by populating them itself
FetchContent_MakeAvailable(Unity)

# Archivos fuente
file(GLOB_RECURSE SOURCES "src/*.c")

# Ejecutable principal
add_executable(${PROJECT_NAME} ${SOURCES})

# Subdirectorio de la librería de memoria
add_subdirectory(lib/memory)

target_include_directories(${PROJECT_NAME} PUBLIC lib/memory/include)

# Link con la librería de memoria y con los libs de Conan
target_link_libraries(${PROJECT_NAME} MemoryLib)

# Si se habilitan tests o cobertura
if(RUN_TESTS EQUAL 1 OR RUN_COVERAGE EQUAL 1)
    add_subdirectory(tests)
endif()

