/**
 * @file expose_metrics.h
 * @brief Programa para leer el uso de CPU y memoria y exponerlos como métricas de Prometheus.
 */
#pragma once

#include "metrics.h"
#include "prom.h"
#include <errno.h>
#include <promhttp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Para sleep

/**
 * Buffer size for reading files
 */
#define BUFFER_SIZE 256
/**
 * @brief Actualiza la métrica de cambios de contexto.
 *
 */
void update_ctxt_gauge();
/**
 * @brief Actualiza la métrica de uso de red de ethernet (Transmision).
 *
 */
void update_net_TX_gauge();
/**
 * @brief Actualiza la métrica de uso de red de ethernet (Recepción).
 *
 */
void update_net_RX_gauge();
/**
 * @brief Actualiza la métrica de número de procesos.
 *
 */
void update_proccesses_gauge();
/**
 * @brief Actualiza la métrica de tiempo de IO.
 *
 */
void update_IO_gauge();
/**
 * @brief Actualiza la métrica de valor de memoria libre.
 */
void update_free_memory_gauge();

/**
 * @brief Actualiza la métrica de uso de CPU.
 */
void update_cpu_gauge();

/**
 * @brief Actualiza la métrica de uso de memoria.
 */
void update_memory_gauge();

/**
 * @brief Función del hilo para exponer las métricas vía HTTP en el puerto 8000.
 * @param arg Argumento no utilizado.
 * @return NULL
 */
void* expose_metrics(void* arg);

/**
 * @brief Inicializar mutex y métricas.
 */
int init_metrics();

/**
 * @brief Destructor de mutex
 */
void destroy_mutex();
/*+
 *  @brief Actualiza las métricas de memoria con los tiempos de ejecución de las políticas de asignación y la tasa de
 * fragmentación.
 */
void update_memory_metrics(double bf_time, double ff_time, double wf_time, double frag_rate);
