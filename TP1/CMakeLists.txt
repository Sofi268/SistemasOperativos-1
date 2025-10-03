cmake_minimum_required(VERSION 3.16)
project(TP1 C)

set(CMAKE_C_STANDARD 11)

# --- Rutas de includes ---
include_directories(
    ${PROJECT_SOURCE_DIR}/include            # headers del TP1
    ${PROJECT_SOURCE_DIR}/lib/memory/include # headers de Memory
    /usr/local/include                       # headers de prometheus-client-c
)

# --- Ejecutable principal ---
set(SOURCES
    src/main.c
    src/expose_metrics.c
    src/metrics.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

# --- Librería Memory ---
add_library(MemoryLib lib/memory/src/memory.c)

# --- Linkeo ---
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        MemoryLib
        prom
        promhttp
        microhttpd
        pthread
)
