cmake_minimum_required(VERSION 3.10)
project(monitoring_project)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/include
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/src
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/promhttp/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/memory/include)

file(GLOB PROM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/src/*.c")
file(GLOB PROMHTTP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/prom/promhttp/src/*.c")

add_library(prom STATIC ${PROM_SOURCES})
add_library(promhttp STATIC ${PROMHTTP_SOURCES})

# Compilar la librería memory
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/memory)

add_executable(monitoring_project src/main.c src/metrics.c src/expose_metrics.c)
add_executable(tp3logic src/test_pol.c)

target_link_libraries(monitoring_project prom promhttp memory microhttpd)
target_link_libraries(tp3logic prom promhttp memory microhttpd)
