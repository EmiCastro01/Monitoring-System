#include "../include/metrics.h"
/**
 * @brief Get the context switches from /proc/stat
 *
 * @return double
 */
double get_ctxt()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long ctxt_n;
    fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/stat");
        return -1;
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (sscanf(buffer, "ctxt %llu", &ctxt_n) == 1)
        {
            break;
        }
    }
    return ctxt_n;
}

/**
 * @brief Get the net RX from /proc/net/dev
 *
 * @return double
 */
double get_net_RX()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long net_RX;
    fp = fopen("/proc/net/dev", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/net/dev");
        return -1;
    }
    fgets(buffer, sizeof(buffer), fp); // Ignorar las primeras lineas de info
    fgets(buffer, sizeof(buffer), fp);
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        unsigned long long temp_net_RX;
        if (sscanf(buffer, "enp0s3: %*u %llu", &temp_net_RX) == 1)
        {
            net_RX = temp_net_RX;
            break;
        }
    }

    fclose(fp);
    return net_RX;
}
/**
 * @brief Get the net TX from /proc/net/dev
 *
 * @return double
 */
double get_net_TX()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long net_TX;
    fp = fopen("/proc/net/dev", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/net/dev");
        return -1;
    }
    fgets(buffer, sizeof(buffer), fp); // Ignorar las primeras lineas de info
    fgets(buffer, sizeof(buffer), fp);
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        unsigned long long temp_net_TX;
        if (sscanf(buffer, "enp0s3: %*u %*u %*u %*u %*u %*u %*u %*u %*u %llu", &temp_net_TX) == 1)
        {
            net_TX = temp_net_TX;
            break;
        }
    }
    fclose(fp);
    return net_TX;
}
/**
 * @brief Get the number of processes from /proc
 *
 * @return double
 */
double get_proccesses()
{
    DIR* dir;
    unsigned long long pr = 0;
    struct dirent* entry;
    dir = opendir("/proc");
    if (dir == NULL)
    {
        perror("Error al abrir /proc");
        return -1;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (isdigit(*entry->d_name))
        {
            pr++;
        }
    }
    closedir(dir);
    return pr;
}
/**
 * @brief Get the IO time from /proc/diskstats
 *
 * @return double
 */
double get_IO_time()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long IO_time;
    fp = fopen("/proc/diskstats", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/diskstats");
        return -1;
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        unsigned long long temp_ruse, temp_wuse, ruse = 0, wuse = 0;
        int matched =
            sscanf(buffer, "%*d %*d %*s %*d %*d %*d %llu %*d %*d %*d %llu %*d %*d %*d %*d %*d %*d %*d %*d %*d",
                   &temp_ruse, &temp_wuse);
        if (matched == 2)
        {
            ruse += temp_ruse;
            wuse += temp_wuse;
            IO_time = ruse + wuse;
        }
    }
    fclose(fp);
    return IO_time;
}
/**
 * @brief Get the free memory from /proc/meminfo
 *
 * @return double
 */
double get_free_memory()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long free_mem;
    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/meminfo");
        return -1;
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (sscanf(buffer, "MemFree: %llu kB", &free_mem) == 1)
        {
            break;
        }
    }
    fclose(fp);
    return free_mem;
}
/**
 * @brief Get the memory usage in percentage from /proc/meminfo
 *
 * @return double
 */
double get_memory_usage()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long total_mem = 0, free_mem = 0;

    // Abrir el archivo /proc/meminfo
    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/meminfo");
        return -1.0;
    }

    // Leer los valores de memoria total y disponible
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (sscanf(buffer, "MemTotal: %llu kB", &total_mem) == 1)
        {
            continue; // MemTotal encontrado
        }
        if (sscanf(buffer, "MemAvailable: %llu kB", &free_mem) == 1)
        {
            break; // MemAvailable encontrado, podemos dejar de leer
        }
    }

    fclose(fp);

    // Verificar si se encontraron ambos valores
    if (total_mem == 0 || free_mem == 0)
    {
        fprintf(stderr, "Error al leer la información de memoria desde /proc/meminfo\n");
        return -1.0;
    }

    // Calcular el porcentaje de uso de memoria
    double used_mem = total_mem - free_mem;
    double mem_usage_percent = (used_mem / total_mem) * 100.0;

    return mem_usage_percent;
}
/**
 * @brief Get the cpu usage in percentage from /proc/stat
 *
 * @return double
 */
double get_cpu_usage()
{
    static unsigned long long prev_user = 0, prev_nice = 0, prev_system = 0, prev_idle = 0, prev_iowait = 0,
                              prev_irq = 0, prev_softirq = 0, prev_steal = 0;
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    unsigned long long totald, idled;
    double cpu_usage_percent;

    // Abrir el archivo /proc/stat
    FILE* fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/stat");
        return -1.0;
    }

    char buffer[BUFFER_SIZE * 4];
    if (fgets(buffer, sizeof(buffer), fp) == NULL)
    {
        perror("Error al leer /proc/stat");
        fclose(fp);
        return -1.0;
    }
    fclose(fp);

    // Analizar los valores de tiempo de CPU
    int ret = sscanf(buffer, "cpu  %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &system, &idle, &iowait,
                     &irq, &softirq, &steal);
    if (ret < 8)
    {
        fprintf(stderr, "Error al parsear /proc/stat\n");
        return -1.0;
    }

    // Calcular las diferencias entre las lecturas actuales y anteriores
    unsigned long long prev_idle_total = prev_idle + prev_iowait;
    unsigned long long idle_total = idle + iowait;

    unsigned long long prev_non_idle = prev_user + prev_nice + prev_system + prev_irq + prev_softirq + prev_steal;
    unsigned long long non_idle = user + nice + system + irq + softirq + steal;

    unsigned long long prev_total = prev_idle_total + prev_non_idle;
    unsigned long long total = idle_total + non_idle;

    totald = total - prev_total;
    idled = idle_total - prev_idle_total;

    if (totald == 0)
    {
        fprintf(stderr, "Totald es cero, no se puede calcular el uso de CPU!\n");
        return -1.0;
    }

    // Calcular el porcentaje de uso de CPU
    cpu_usage_percent = ((double)(totald - idled) / totald) * 100.0;

    // Actualizar los valores anteriores para la siguiente lectura
    prev_user = user;
    prev_nice = nice;
    prev_system = system;
    prev_idle = idle;
    prev_iowait = iowait;
    prev_irq = irq;
    prev_softirq = softirq;
    prev_steal = steal;

    return cpu_usage_percent;
}
