#include "../include/expose_metrics.h"

/** Mutex para sincronización de hilos */
pthread_mutex_t lock;

/** Métrica de Prometheus para el uso de CPU */
static prom_gauge_t* cpu_usage_metric = NULL;

/** Métrica de Prometheus para el uso de memoria */
static prom_gauge_t* memory_usage_metric = NULL;

/** Métrica de Prometheus para el valor libre de memoria */
static prom_gauge_t* free_memory_metric = NULL;

/** Métrica de Prometheus para el tiempo de escritura y lectura */
static prom_gauge_t* IO_time_metric = NULL;

/** Métrica de Prometheus para el número de procesos */
static prom_gauge_t* proccesses_metric = NULL;

/** Métrica de Prometheus para el uso de red de ethernet (transmisión de paquetes) */
static prom_gauge_t* extern_net_TX_metric = NULL;

/** Métrica de Prometheus para el uso de red de ethernet (recepción de paquetes) */
static prom_gauge_t* extern_net_RX_metric = NULL;

/** Métrica de Prometheus para los cambios de contexto */
static prom_gauge_t* context_switch_metric = NULL;

/**
 * @brief Update the context switch metric
 *
 */
void update_ctxt_gauge()
{
    if (context_switch_metric == NULL)
    {
        return;
    }
    double ctxt = get_ctxt();
    if (ctxt >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(context_switch_metric, ctxt, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el número de cambios de contexto\n");
    }
}

/**
 * @brief Update the net RX metric
 *
 */
void update_net_RX_gauge()
{
    if (extern_net_RX_metric == NULL)
    {
        return;
    }
    double net_RX = get_net_RX();
    if (net_RX >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(extern_net_RX_metric, net_RX, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr,
                "%f"
                "No hubo recepción de paquetes: ",
                net_RX);
    }
}

/**
 * @brief Update the TX metric
 *
 */
void update_net_TX_gauge()
{
    if (extern_net_TX_metric == NULL)
    {
        return;
    }
    double net_TX = get_net_TX();
    if (net_TX >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(extern_net_TX_metric, net_TX, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr,
                "%f"
                "No hubo transmision de paquetes: ",
                net_TX);
    }
}

/**
 * @brief Update the number of proccesses metric
 *
 */
void update_proccesses_gauge()
{
    if (proccesses_metric == NULL)
    {
        return;
    }
    double proccesses = get_proccesses();
    if (proccesses >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(proccesses_metric, proccesses, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el numero de procesos\n");
    }
}

/**
 * @brief Update the IO operations metric
 *
 */
void update_IO_gauge()
{
    if (IO_time_metric == NULL)
    {
        return;
    }
    double IO_time = get_IO_time();
    if (IO_time >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(IO_time_metric, IO_time, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el tiempo de IO\n");
    }
}

/**
 * @brief Update the free memory metric
 *
 */
void update_free_memory_gauge()
{
    if (free_memory_metric == NULL)
    {
        return;
    }
    double free_mem = get_free_memory();
    if (free_mem >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(free_memory_metric, free_mem, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el valor libre de memoria\n");
    }
}

/**
 * @brief Update the cpu usage metric
 *
 */
void update_cpu_gauge()
{
    if (cpu_usage_metric == NULL)
    {
        return;
    }
    double usage = get_cpu_usage();
    if (usage >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(cpu_usage_metric, usage, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el uso de CPU\n");
    }
}

/**
 * @brief Update the memory usage metric
 *
 */
void update_memory_gauge()
{
    if (memory_usage_metric == NULL)
    {
        return;
    }
    double usage = get_memory_usage();
    if (usage >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(memory_usage_metric, usage, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el uso de memoria\n");
    }
}

/**
 * @brief execute server to expose metrics and keep it running
 *
 */
void* expose_metrics(void* arg)
{
    (void)arg; // Argumento no utilizado

    // Aseguramos que el manejador HTTP esté adjunto al registro por defecto
    promhttp_set_active_collector_registry(NULL);

    // Iniciamos el servidor HTTP en el puerto 8000
    struct MHD_Daemon* daemon = promhttp_start_daemon(MHD_USE_SELECT_INTERNALLY, 8000, NULL, NULL);
    if (daemon == NULL)
    {
        fprintf(stderr, "Error al iniciar el servidor HTTP\n");
        return NULL;
    }

    // Mantenemos el servidor en ejecución
    while (1)
    {
        sleep(1);
    }

    // Nunca debería llegar aquí
    MHD_stop_daemon(daemon);
    return NULL;
}

/**
 * @brief initialize metrics
 */
int init_metrics()
{
    // Inicializamos el mutex
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        fprintf(stderr, "Error al inicializar el mutex\n");
        return EXIT_FAILURE;
    }

    // Inicializamos el registro de coleccionistas de Prometheus
    if (prom_collector_registry_default_init() != 0)
    {
        fprintf(stderr, "Error al inicializar el registro de Prometheus\n");
        return EXIT_FAILURE;
    }

    // Creamos la métrica para el uso de CPU
    cpu_usage_metric = prom_gauge_new("cpu_usage_percentage", "Porcentaje de uso de CPU", 0, NULL);
    if (cpu_usage_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de uso de CPU\n");
        return EXIT_FAILURE;
    }

    // Creamos la métrica para el uso de memoria
    memory_usage_metric = prom_gauge_new("memory_usage_percentage", "Porcentaje de uso de memoria", 0, NULL);
    if (memory_usage_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de uso de memoria\n");
        return EXIT_FAILURE;
    }

    free_memory_metric = prom_gauge_new("free_memory_val", "Memoria libre", 0, NULL);
    if (free_memory_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de uso de memoria\n");
        return EXIT_FAILURE;
    }

    IO_time_metric = prom_gauge_new("IO_time", "Tiempo de escritura + lectura de IO", 0, NULL);
    if (IO_time_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de IO\n");
        return EXIT_FAILURE;
    }

    proccesses_metric = prom_gauge_new("proccesses", "Numero de procesos", 0, NULL);
    if (proccesses_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de procesos\n");
        return EXIT_FAILURE;
    }

    extern_net_TX_metric = prom_gauge_new("extern_net_TX", "Uso de red de ethernet (TX)", 0, NULL);
    if (extern_net_TX_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de uso de red de ethernet (TX)\n");
        return EXIT_FAILURE;
    }

    extern_net_RX_metric = prom_gauge_new("extern_net_RX", "Uso de red de ethernet (RX)", 0, NULL);
    if (extern_net_RX_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de uso de red de ethernet (RX)\n");
        return EXIT_FAILURE;
    }

    context_switch_metric = prom_gauge_new("context_switch", "Numero de cambios de contexto", 0, NULL);
    if (context_switch_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de cambios de contexto\n");
        return EXIT_FAILURE;
    }
    // Registramos las métricas en el registro por defecto
    if (prom_collector_registry_must_register_metric(cpu_usage_metric) == NULL ||
        prom_collector_registry_must_register_metric(memory_usage_metric) == NULL ||
        prom_collector_registry_must_register_metric(free_memory_metric) == NULL ||
        prom_collector_registry_must_register_metric(IO_time_metric) == NULL ||
        prom_collector_registry_must_register_metric(proccesses_metric) == NULL ||
        prom_collector_registry_must_register_metric(extern_net_TX_metric) == NULL ||
        prom_collector_registry_must_register_metric(extern_net_RX_metric) == NULL ||
        prom_collector_registry_must_register_metric(context_switch_metric) == NULL)
    {
        fprintf(stderr, "Error al registrar las métricas\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 * @brief destroy mutex
 *
 */
void destroy_mutex()
{
    pthread_mutex_destroy(&lock);
}