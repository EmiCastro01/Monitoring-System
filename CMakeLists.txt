cmake_minimum_required(VERSION 3.10)
project(monitoring_project)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/include
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/src
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/promhttp/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../lib/memory/include # Para memory.h
)

file(GLOB PROM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/src/*.c")
file(GLOB PROMHTTP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/prom/promhttp/src/*.c")

add_library(prom STATIC ${PROM_SOURCES})
add_library(promhttp STATIC ${PROMHTTP_SOURCES})

# Incluir archivos de TP3 (test_politica y memory)
add_library(
  tp3logic STATIC src/tp3_logic.c
                  ${CMAKE_CURRENT_SOURCE_DIR}/../lib/memory/src/memory.c)

add_executable(monitoring_project src/main.c src/metrics.c src/expose_metrics.c)

target_link_libraries(monitoring_project prom promhttp test_pol)

set_target_properties(monitoring_project PROPERTIES C_STANDARD 11)
