#include "metrics.h"

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

    char buffer[MULTI_BUFFER_SIZE];
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
    if (ret < CPU_STATS_COUNT)
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

double get_free_memory()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long free_mem = 0;

    // Abrir el archivo /proc/meminfo
    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/meminfo");
        return -1.0;
    }

    // Leer los valores de memoria disponible
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (sscanf(buffer, "MemAvailable: %llu kB", &free_mem) == 1)
        {
            break; // MemAvailable encontrado, podemos dejar de leer
        }
    }

    fclose(fp);

    // Verificar si se encontro el valor
    if (free_mem == 0)
    {
        fprintf(stderr, "Error al leer la información de memoria disponible desde /proc/meminfo\n");
        return -1.0;
    }

    return free_mem;
}

double get_total_memory()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long total_mem = 0;

    // Abrir el archivo /proc/meminfo
    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/meminfo");
        return -1.0;
    }

    // Leer los valores de memoria disponible
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (sscanf(buffer, "MemTotal: %llu kB", &total_mem) == 1)
        {
            break; // MemTotal encontrado, podemos dejar de leer
        }
    }

    fclose(fp);

    // Verificar si se encontro el valor
    if (total_mem == 0)
    {
        fprintf(stderr, "Error al leer la información de memoria total desde /proc/meminfo\n");
        return -1.0;
    }

    return total_mem;
}

double get_used_memory()
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
    return used_mem;
}

double get_disk_reads()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long reads_completed;
    unsigned long long total_reads = 0; // Almacena la suma total
    const char* target_prefix = "sda";  // Prefijo de disco

    // Abrir el archivo /proc/diskstats
    fp = fopen("/proc/diskstats", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/diskstats");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (strstr(buffer, target_prefix) != NULL)
        {
            if (sscanf(buffer, "%*d %*d %*s %llu", &reads_completed) == 1)
            { // Ignora los primeros valores ya que no son relevantes
                total_reads += reads_completed;
            }
        }
    }

    fclose(fp);

    return total_reads;
}

double get_loop_reads()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long reads_completed;
    unsigned long long total_reads = 0;
    const char* target_prefix = "loop"; // Prefijo de dispositivo de montado de archivos

    fp = fopen("/proc/diskstats", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/diskstats");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (strstr(buffer, target_prefix) != NULL)
        {
            if (sscanf(buffer, "%*d %*d %*s %llu", &reads_completed) == 1)
            {
                total_reads += reads_completed;
            }
        }
    }

    fclose(fp);

    return total_reads;
}

double get_disk_writes()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long writes_completed;
    unsigned long long total_writes = 0;
    const char* target_prefix = "sda";

    fp = fopen("/proc/diskstats", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/diskstats");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (strstr(buffer, target_prefix) != NULL)
        {
            if (sscanf(buffer, "%*u %*u %*s %*u %llu", &writes_completed) == 1)
            {
                total_writes += writes_completed;
            }
        }
    }

    fclose(fp);

    return total_writes;
}

double get_loop_writes()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long writes_completed;
    unsigned long long total_writes = 0;
    const char* target_prefix = "loop";

    fp = fopen("/proc/diskstats", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/diskstats");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (strstr(buffer, target_prefix) != NULL)
        {
            if (sscanf(buffer, "%*u %*u %*s %*u %*u %*u %*u %llu", &writes_completed) == 1)
            {
                total_writes += writes_completed;
            }
        }
    }

    fclose(fp);

    return total_writes;
}

double get_time_reads()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long time_read = 0;
    unsigned long long time;

    fp = fopen("/proc/diskstats", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/diskstats");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (sscanf(buffer, "%*u %*u %*s %*u %*u %*u %llu ", &time) == 1)
        {
            time_read += time;
        }
    }

    fclose(fp);
    return (double)time_read;
}

double get_time_writes()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long time_write = 0;
    unsigned long long time;

    fp = fopen("/proc/diskstats", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/diskstats");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {

        if (sscanf(buffer, "%*u %*u %*s %*u %*u %*u %*u %llu", &time) == 1)
        {
            time_write += time;
        }
    }

    fclose(fp);
    return (double)time_write;
}

double get_IO_in_progress()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long total_io_in_progress = 0;
    unsigned long long io_in_progress;

    fp = fopen("/proc/diskstats", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/diskstats");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {

        if (sscanf(buffer, "%*u %*u %*s %*u %*u %*u %*u %*u %*u %*u %*u %llu", &io_in_progress) == 1)
        {
            total_io_in_progress += io_in_progress;
        }
    }

    fclose(fp);
    return total_io_in_progress;
}

double get_time_in_IO()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long time_io = 0;
    unsigned long long time;
    fp = fopen("/proc/diskstats", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/diskstats");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (sscanf(buffer, "%*u %*u %*s %*u %*u %*u %*u %*u %*u %*u %*u %*u %llu", &time) == 1)
        {
            time_io += time;
        }
    }

    fclose(fp);
    return (double)time_io;
}

int is_num(const char* process)
{
    while (*process)
    {
        if (!isdigit(*process))
        {
            return 0;
        }
        process++;
    }
    return 1;
}

double get_num_processes()
{
    unsigned long process = 0;

    // Abre el directorio /proc
    DIR* fp = opendir("/proc");
    if (fp == NULL)
    {
        perror("Error al abrir /proc");
        return -1;
    }

    // Recorre las entradas del directorio
    struct dirent* entrada;
    while ((entrada = readdir(fp)) != NULL)
    {
        // Comprueba si el nombre de la entrada es un número
        if (is_num(entrada->d_name))
        {
            process++;
        }
    }

    closedir(fp);
    return (double)process;
}

double get_received_bytes()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long received_bytes = 0;
    unsigned long long bytes;
    const char* target_prefix = "en";

    fp = fopen("/proc/net/dev", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/net/dev");
        return -1;
    }

    // Saltar la primera línea (encabezado)
    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    // Leer las líneas de las interfaces
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // Verifica si el nombre de la interfaz comienza con el prefijo "en"
        if (strstr(buffer, target_prefix) != NULL)
        {
            // Usar sscanf para extraer los bytes recibidos
            if (sscanf(buffer, "%*s %llu", &bytes) == 1)
            {
                received_bytes += bytes;
            }
        }
    }

    fclose(fp);
    return (double)received_bytes;
}

double get_sent_bytes()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long sent_bytes = 0;
    unsigned long long bytes;
    const char* target_prefix = "en";

    fp = fopen("/proc/net/dev", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/net/dev");
        return -1;
    }

    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    // Leer las líneas de las interfaces
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // Verifica si el nombre de la interfaz comienza con el prefijo "en"
        if (strstr(buffer, target_prefix) != NULL)
        {
            if (sscanf(buffer, "%*s %*u %*u %*u %*u %*u %*u %*u %*u %llu", &bytes) == 1)
            {
                sent_bytes += bytes;
            }
        }
    }

    fclose(fp);
    return (double)sent_bytes;
}
double get_received_packets()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long received_packets = 0;
    unsigned long long packets;
    const char* target_prefix = "en";

    fp = fopen("/proc/net/dev", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/net/dev");
        return -1;
    }

    // Saltar la primera línea (encabezado)
    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    // Leer las líneas de las interfaces
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // Verifica si el nombre de la interfaz comienza con el prefijo "en"
        if (strstr(buffer, target_prefix) != NULL)
        {
            // Usar sscanf para extraer los paquetes recibidos
            if (sscanf(buffer, "%*s %*u %llu", &packets) == 1)
            {
                received_packets += packets;
            }
        }
    }

    fclose(fp);
    return (double)received_packets;
}
double get_sent_packets()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long sent_packets = 0;
    unsigned long long packets;
    const char* target_prefix = "en";

    fp = fopen("/proc/net/dev", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/net/dev");
        return -1;
    }

    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    // Leer las líneas de las interfaces
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // Verifica si el nombre de la interfaz comienza con el prefijo "en"
        if (strstr(buffer, target_prefix) != NULL)
        {
            if (sscanf(buffer, "%*s %*u %*u %*u %*u %*u %*u %*u %*u %*u %llu", &packets) == 1)
            {
                sent_packets += packets;
            }
        }
    }

    fclose(fp);
    return (double)sent_packets;
}

double get_received_errors()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long received_errors = 0;
    unsigned long long errs;
    const char* target_prefix = "en";

    fp = fopen("/proc/net/dev", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/net/dev");
        return -1;
    }

    // Saltar la primera línea (encabezado)
    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    // Leer las líneas de las interfaces
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // Verifica si el nombre de la interfaz comienza con el prefijo "en"
        if (strstr(buffer, target_prefix) != NULL)
        {
            // Usar sscanf para extraer los errores de recepcion
            if (sscanf(buffer, "%*s %*u %*u %llu", &errs) == 1)
            {
                received_errors += errs;
            }
        }
    }

    fclose(fp);
    return (double)received_errors;
}
double get_sent_errors()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long sent_errors = 0;
    unsigned long long errs;
    const char* target_prefix = "en";

    fp = fopen("/proc/net/dev", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/net/dev");
        return -1;
    }

    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    // Leer las líneas de las interfaces
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // Verifica si el nombre de la interfaz comienza con el prefijo "en"
        if (strstr(buffer, target_prefix) != NULL)
        {
            if (sscanf(buffer, "%*s %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %llu", &errs) == 1)
            {
                sent_errors += errs;
            }
        }
    }

    fclose(fp);
    return (double)sent_errors;
}
double get_user_time()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long user_time;
    unsigned long long time = 0;
    const char* target_prefix = "cpu";

    fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/stat");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (strstr(buffer, target_prefix) != NULL)
        {
            if (sscanf(buffer, "%*s %llu", &time) == 1)
            {
                user_time += time;
            }
        }
    }

    fclose(fp);

    return user_time;
}

double get_kernel_time()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long kernel_time;
    unsigned long long time = 0;
    const char* target_prefix = "cpu";

    fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/stat");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (strstr(buffer, target_prefix) != NULL)
        {
            if (sscanf(buffer, "%*s %*u %*u %llu", &time) == 1)
            {
                kernel_time += time;
            }
        }
    }

    fclose(fp);

    return kernel_time;
}

double get_inactive_time()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long idle_time;
    unsigned long long time = 0;
    const char* target_prefix = "cpu";

    fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/stat");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (strstr(buffer, target_prefix) != NULL)
        {
            if (sscanf(buffer, "%*s %*u %*u %*u %llu", &time) == 1)
            {
                idle_time += time;
            }
        }
    }

    fclose(fp);

    return idle_time;
}

double get_IO_wait()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long io_time;
    unsigned long long time = 0;
    const char* target_prefix = "cpu";

    fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/stat");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (strstr(buffer, target_prefix) != NULL)
        {
            if (sscanf(buffer, "%*s %*u %*u %*u %*u %llu", &time) == 1)
            {
                io_time += time;
            }
        }
    }

    fclose(fp);

    return io_time;
}
