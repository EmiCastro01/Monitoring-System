#include "test_pol.h"
#include "cjson/cJSON.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_BLOCKS 1000
#define BLOCK_SIZE_TEST 64

void write_metrics(double bf_time, double ff_time, double wf_time, double frag_rate)
{
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "best_fit_time_ms", bf_time);
    cJSON_AddNumberToObject(root, "first_fit_time_ms", ff_time);
    cJSON_AddNumberToObject(root, "worst_fit_time_ms", wf_time);
    cJSON_AddNumberToObject(root, "fragmentation_rate", frag_rate);

    char* s = cJSON_PrintUnformatted(root);
    FILE* f = fopen("tmp/metrics.json", "w");
    if (f)
    {
        fprintf(f, "%s\n", s);
        fclose(f);
    }
    else
    {
        fprintf(stderr, "Error al escribir /tmp/metrics.json\n");
    }

    cJSON_free(s);
    cJSON_Delete(root);
}

double test_politica(int metodo, const char* nombre)
{
    set_method(metodo);
    clock_t start = clock();

    void* bloques[NUM_BLOCKS];

    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        bloques[i] = malloc(BLOCK_SIZE_TEST);
    }

    for (int i = 0; i < NUM_BLOCKS; i += 2)
    {
        free(bloques[i]);
    }

    for (int i = 0; i < NUM_BLOCKS / 2; i++)
    {
        void* tmp = malloc(BLOCK_SIZE_TEST / 2);
        (void)tmp;
    }

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;

    printf("Política: %s – Tiempo: %.2f ms\n", nombre, elapsed);
    return elapsed;
}

int main()
{
    printf("\n==== Test de asignación de memoria ====\n");

    double t_bf = test_politica(BEST_FIT, "Best Fit");
    double t_ff = test_politica(FIRST_FIT, "First Fit");
    double t_wf = test_politica(WORST_FIT, "Worst Fit");

    write_metrics(t_bf, t_ff, t_wf, 0.0);

    printf("Métricas escritas en tmp/metrics.json\n");

    return 0;
}
