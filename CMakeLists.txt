# shell-personalizada/monitor/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(monitoring_project)

# Accedé a includes de monitor
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Accedé a prom
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/include
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/src
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/promhttp/include)

# ⬇️ Agregá este include path para usar memory.h sin ../../
include_directories(${CMAKE_SOURCE_DIR}/lib/memory/include)

# Compilar prom
file(GLOB PROM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/src/*.c")
file(GLOB PROMHTTP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/prom/promhttp/src/*.c")

add_library(prom STATIC ${PROM_SOURCES})
add_library(promhttp STATIC ${PROMHTTP_SOURCES})

# Executable de monitor
add_executable(monitoring_project src/main.c src/metrics.c src/expose_metrics.c)

# test_pol.c con main
add_executable(tp3logic src/test_pol.c)

# Linkeá todas las dependencias necesarias (incluye memory)
target_link_libraries(monitoring_project prom promhttp memory)
target_link_libraries(tp3logic prom promhttp memory microhttpd)
