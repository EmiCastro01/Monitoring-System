cmake_minimum_required(VERSION 3.10)
project(monitoring_project)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/include
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/src
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/promhttp/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/memory/include)

file(GLOB PROM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/src/*.c")
file(GLOB PROMHTTP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/prom/promhttp/src/*.c")

add_library(prom STATIC ${PROM_SOURCES})
add_library(promhttp STATIC ${PROMHTTP_SOURCES})

# test_pol.c es tu wrapper, y memory.c es la implementación real
add_library(tp3logic STATIC src/test_pol.c ../../../lib/memory/src/memory.c)

add_executable(monitoring_project src/main.c src/metrics.c src/expose_metrics.c)

target_link_libraries(monitoring_project prom promhttp tp3logic)

set_target_properties(monitoring_project PROPERTIES C_STANDARD 11)
