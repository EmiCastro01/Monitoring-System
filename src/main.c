#include "../include/expose_metrics.h"
#include "test_pol.h"
#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>

/** Time to sleep between metric updates */
#define SLEEP_TIME 1

/**
 * @brief entry point (main function)
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char* argv[])
{
    int sample_time = SLEEP_TIME;

    if (argc > 1)
    {
        if (isdigit(*argv[1]))
        {
            sample_time = atoi(argv[1]);
        }
        else
        {
            fprintf(stderr, "El argumento debe ser un número entero\n");
            return EXIT_FAILURE;
        }
    }

    printf("Muestreo de métricas cada %d\n", sample_time);
    init_metrics();

    // Creamos un hilo para exponer las métricas vía HTTP
    pthread_t tid;
    if (pthread_create(&tid, NULL, expose_metrics, NULL) != 0)
    {
        fprintf(stderr, "Error al crear el hilo del servidor HTTP\n");
        return EXIT_FAILURE;
    }

    // Bucle principal para actualizar las métricas cada segundo
    while (true)
    {
        for (int i = 2; i < argc; i++)
        {
            if (strcmp(argv[i], "cpu_usage") == 0)
            {
                update_cpu_gauge();
            }
            else if (strcmp(argv[i], "memory_usage") == 0)
            {
                update_memory_gauge();
            }
            else if (strcmp(argv[i], "free_memory") == 0)
            {
                update_free_memory_gauge();
            }
            else if (strcmp(argv[i], "IO_time") == 0)
            {
                update_IO_gauge();
            }
            else if (strcmp(argv[i], "proccesses") == 0)
            {
                update_proccesses_gauge();
            }
            else if (strcmp(argv[i], "net_TX") == 0)
            {
                update_net_TX_gauge();
            }
            else if (strcmp(argv[i], "net_RX") == 0)
            {
                update_net_RX_gauge();
            }
            else if (strcmp(argv[i], "context_switch") == 0)
            {
                update_ctxt_gauge();
            }
            else if (strcmp(argv[i], "mem_metrics") == 0)
            {
                update_memory_metrics(test_politica(0, "best"), test_politica(1, "first"), test_politica(2, "worst"),
                                      0.0);
            }

            else
            {
                fprintf(stderr, "Métrica desconocida: %s\n", argv[i]);
            }
        }
        sleep(sample_time);
    }

    return EXIT_SUCCESS;
}
