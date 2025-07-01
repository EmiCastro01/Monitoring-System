cmake_minimum_required(VERSION 3.10)
project(monitoring_project)

# Includes de monitor
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Includes de prom
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/include
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/src
  ${CMAKE_CURRENT_SOURCE_DIR}/prom/promhttp/include)

# Include de memory para usar funciones como set_method
include_directories(${CMAKE_SOURCE_DIR}/lib/memory/include)

# Archivos fuente de prom y promhttp
file(GLOB PROM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/prom/prom/src/*.c")
file(GLOB PROMHTTP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/prom/promhttp/src/*.c")

# Compilar librerías estáticas
add_library(prom STATIC ${PROM_SOURCES})
add_library(promhttp STATIC ${PROMHTTP_SOURCES})

# Ejecutables
add_executable(monitoring_project src/main.c src/metrics.c src/expose_metrics.c
                                  src/test_pol.c)

add_executable(tp3logic src/test_pol.c src/metrics.c src/expose_metrics.c)

# Linkeo de dependencias

target_link_libraries(monitoring_project prom promhttp microhttpd memory cjson)
target_link_libraries(tp3logic prom promhttp microhttpd memory cjson)
