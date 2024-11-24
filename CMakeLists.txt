cmake_minimum_required(VERSION 3.10)
project(monitoring_project)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(monitoring_project
        src/main.c
        src/metrics.c
        src/expose_metrics.c
)

link_directories(${CMAKE_SOURCE_DIR}/lib)
find_library(PROM_LIBRARY prom REQUIRED)
find_library(PROMHTTP_LIBRARY promhttp REQUIRED)
find_library(MICROHTTPD_LIBRARY microhttpd REQUIRED)

target_link_libraries(monitoring_project
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libprom.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libpromhttp.so
)
