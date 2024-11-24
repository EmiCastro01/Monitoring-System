/**
 * @file metrics.h
 * @brief Funciones para obtener el uso de CPU y memoria desde el sistema de archivos /proc.
 */
#pragma once

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/**
 * @brief Buffer size for reading files from /proc.
 */

#define BUFFER_SIZE 256

/**
 * @brief Obtiene el porcentaje de uso de memoria desde /proc/meminfo.
 *
 * Lee los valores de memoria total y disponible desde /proc/meminfo y calcula
 * el porcentaje de uso de memoria.
 *
 * @return Uso de memoria como porcentaje (0.0 a 100.0), o -1.0 en caso de error.
 */
double get_memory_usage();

/**
 * @brief Obtiene el porcentaje de uso de CPU desde /proc/stat.
 *
 * Lee los tiempos de CPU desde /proc/stat y calcula el porcentaje de uso de CPU
 * en un intervalo de tiempo.
 *
 * @return Uso de CPU como porcentaje (0.0 a 100.0), o -1.0 en caso de error.
 */
double get_cpu_usage();

/**
 * @brief Obtiene el número de cambios de contexto desde /proc/stat.
 *
 * Lee el número de cambios de contexto desde /proc/stat.
 *
 * @return Número de cambios de contexto, o -1 en caso de error.
 */
double get_ctxt();

/**
 * @brief Obtiene el número de paquetes recibidos desde /proc/net/dev.
 *
 * Lee el número de paquetes recibidos desde /proc/net/dev.
 *
 * @return Número de paquetes recibidos, o -1 en caso de error.
 */

double get_net_RX();

/**
 * @brief Obtiene el número de paquetes transmitidos desde /proc/net/dev.
 *
 * Lee el número de paquetes transmitidos desde /proc/net/dev.
 *
 * @return Número de paquetes transmitidos, o -1 en caso de error.
 */

double get_net_TX();

/**
 * @brief Obtiene el tiempo de escritura y lectura de IO desde /proc/diskstats.
 *
 * Lee el tiempo de escritura y lectura de IO desde /proc/diskstats.
 *
 * @return Tiempo de escritura y lectura de IO, o -1 en caso de error.
 */

double get_IO_time();

/**
 * @brief Obtiene el valor libre de memoria desde /proc/meminfo.
 *
 * Lee el valor de memoria libre desde /proc/meminfo.
 *
 * @return Valor libre de memoria, o -1 en caso de error.
 */

double get_free_memory();

/**
 * @brief Obtiene el número de procesos desde /proc/stat.
 *
 * Lee el número de procesos desde /proc/stat.
 *
 * @return Número de procesos, o -1 en caso de error.
 */

double get_proccesses();
