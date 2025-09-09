#include "expose_metrics.h"

/** Mutex para sincronización de hilos */
pthread_mutex_t lock;

/** Métrica de Prometheus para el uso de CPU */
static prom_gauge_t* cpu_usage_metric;

/** Métrica de Prometheus para el uso de memoria */
static prom_gauge_t* memory_usage_metric;

/** Métrica de Prometheus para la memoria libre*/
static prom_gauge_t* free_memory_metric;

/** Métrica de Prometheus para la memoria en uso */
static prom_gauge_t* used_memory_metric;

/** Métrica de Prometheus para la memoria total */
static prom_gauge_t* total_memory_metric;

/** Métrica de Prometheus para la lectura de disco */
static prom_gauge_t* disk_reads_metric;

/** Métrica de Prometheus para la lectura de dispositivos de montaje */
static prom_gauge_t* loop_reads_metric;

/** Métrica de Prometheus para la escritura de disco */
static prom_gauge_t* disk_writes_metric;

/** Métrica de Prometheus para la escritura de montaje de archivos */
static prom_gauge_t* loop_writes_metric;

/** Métrica de Prometheus para el tiempo de lectura de dispositivos de E/S */
static prom_gauge_t* time_reads_metric;

/** Métrica de Prometheus para el tiempo de escritura de dispositivos de E/S */
static prom_gauge_t* time_writes_metric;

/** Métrica de Prometheus para la cantidad de operaciones de E/S */
static prom_gauge_t* IO_in_progress_metric;

/** Métrica de Prometheus para el tiempo de operaciones de E/S*/
static prom_gauge_t* time_IO_metric;

/** Métrica de Prometheus para la cantidad de procesos en ejecucion */
static prom_gauge_t* num_processes_metric;
//** Métrica de Prometheus para la cantidad de procesos en ejecución */
static prom_gauge_t* num_processes_metric;

/** Métrica de Prometheus para los bytes recibidos */
static prom_gauge_t* received_bytes_metric;

/** Métrica de Prometheus para los bytes enviados */
static prom_gauge_t* sent_bytes_metric;

/** Métrica de Prometheus para los paquetes recibidos */
static prom_gauge_t* received_packets_metric;

/** Métrica de Prometheus para los paquetes enviados */
static prom_gauge_t* sent_packets_metric;

/** Métrica de Prometheus para los errores de recepción */
static prom_gauge_t* received_errors_metric;

/** Métrica de Prometheus para los errores de envío */
static prom_gauge_t* sent_errors_metric;

/** Métrica de Prometheus para el tiempo de CPU en modo usuario */
static prom_gauge_t* user_time_metric;

/** Métrica de Prometheus para el tiempo de CPU en modo kernel */
static prom_gauge_t* kernel_time_metric;

/** Métrica de Prometheus para el tiempo de inactividad de la CPU */
static prom_gauge_t* inactive_time_metric;

/** Métrica de Prometheus para el tiempo de espera de entrada/salida de la CPU */
static prom_gauge_t* io_wait_metric;

void update_cpu_gauge()
{
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

void update_memory_gauge()
{
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

void update_free_memory_gauge()
{
    double free = get_free_memory();
    if (free >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(free_memory_metric, free, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el uso de memoria disponible\n");
    }
}

void update_used_memory_gauge()
{
    double used = get_used_memory();
    if (used >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(used_memory_metric, used, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el uso de memoria en uso\n");
    }
}

void set_total_memory_gauge()
{
    double total = get_total_memory();
    if (total >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(total_memory_metric, total, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el uso de memoria total\n");
    }
}

void update_disk_reads()
{
    double reads = get_disk_reads();
    if (reads >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(disk_reads_metric, reads, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener las lecturas de disco \n");
    }
}

void update_loop_reads()
{
    double reads = get_loop_reads();
    if (reads >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(loop_reads_metric, reads, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener las lecturas de dispositivos de montaje\n");
    }
}

void update_disk_writes()
{
    double writes = get_disk_writes();
    if (writes >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(disk_writes_metric, writes, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener las escritura de disco \n");
    }
}

void update_loop_writes()
{
    double writes = get_loop_writes();
    if (writes >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(loop_writes_metric, writes, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener las lecturas de dispositivos de montaje \n");
    }
}

void update_time_reads()
{
    double time_reads = get_time_reads();
    if (time_reads >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(time_reads_metric, time_reads, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el tiempo lecturas de dispositivos \n");
    }
}

void update_time_writes()
{
    double time_writes = get_time_writes();
    if (time_writes >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(time_writes_metric, time_writes, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el tiempo de escritura de dispositivos \n");
    }
}

void update_IO_in_progress()
{
    double num = get_IO_in_progress();
    if (num >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(IO_in_progress_metric, num, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener las operaciones sobre dispositivos de E/S \n");
    }
}

void update_time_in_IO()
{
    double time = get_time_in_IO();
    if (time >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(time_IO_metric, time, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el tiempo de operaciones de E/S \n");
    }
}

void update_num_processes()
{
    double num = get_num_processes();
    if (num >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(num_processes_metric, num, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener la cantidad de procesos en ejecucion \n");
    }
}

void update_received_bytes()
{
    double bytes = get_received_bytes();
    if (bytes >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(received_bytes_metric, bytes, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener los bytes recibidos\n");
    }
}

void update_sent_bytes()
{
    double bytes = get_sent_bytes();
    if (bytes >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(sent_bytes_metric, bytes, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener los bytes enviados\n");
    }
}

void update_received_packets()
{
    double packets = get_received_packets();
    if (packets >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(received_packets_metric, packets, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener los paquetes recibidos\n");
    }
}

void update_sent_packets()
{
    double packets = get_sent_packets();
    if (packets >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(sent_packets_metric, packets, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener los paquetes enviados\n");
    }
}

void update_received_errors()
{
    double errors = get_received_errors();
    if (errors >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(received_errors_metric, errors, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener los errores de recepción\n");
    }
}

void update_sent_errors()
{
    double errors = get_sent_errors();
    if (errors >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(sent_errors_metric, errors, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener los errores de envío\n");
    }
}

void update_user_time()
{
    double user_time = get_user_time();
    if (user_time >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(user_time_metric, user_time, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el tiempo de CPU en modo usuario\n");
    }
}

void update_kernel_time()
{
    double kernel_time = get_kernel_time();
    if (kernel_time >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(kernel_time_metric, kernel_time, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el tiempo de CPU en modo kernel\n");
    }
}

void update_inactive_time()
{
    double inactive_time = get_inactive_time();
    if (inactive_time >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(inactive_time_metric, inactive_time, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el tiempo de inactividad de la CPU\n");
    }
}

void update_IO_wait()
{
    double io_wait = get_IO_wait();
    if (io_wait >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(io_wait_metric, io_wait, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el tiempo de espera de I/O\n");
    }
}

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
        sleep(SLEEP_TIME);
    }

    // Nunca debería llegar aquí
    MHD_stop_daemon(daemon);
    return NULL;
}

void init_metrics()
{
    // Inicializamos el mutex
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        fprintf(stderr, "Error al inicializar el mutex\n");
        return;
    }

    // Inicializamos el registro de coleccionistas de Prometheus
    if (prom_collector_registry_default_init() != 0)
    {
        fprintf(stderr, "Error al inicializar el registro de Prometheus\n");
        return;
    }

    // Creamos la métrica para el uso de CPU
    cpu_usage_metric = prom_gauge_new("cpu_usage_percentage", "Porcentaje de uso de CPU", 0, NULL);
    if (cpu_usage_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de uso de CPU\n");
        return;
    }

    // Creamos la métrica para el uso de memoria
    memory_usage_metric = prom_gauge_new("memory_usage_percentage", "Porcentaje de uso de memoria", 0, NULL);
    if (memory_usage_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de uso de memoria\n");
        return;
    }

    // Creamos la métrica para la memoria disponible
    free_memory_metric = prom_gauge_new("free_memory_metric", "Memoria Disponible", 0, NULL);
    if (free_memory_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de memoria disponible\n");
        return;
    }

    // Creamos la métrica para el uso de memoria
    used_memory_metric = prom_gauge_new("used_memory_metric", "Memoria en Uso", 0, NULL);
    if (used_memory_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de memoria en uso\n");
        return;
    }

    // Creamos la métrica para la memoria total
    total_memory_metric = prom_gauge_new("total_memory_metric", "Memoria Total", 0, NULL);
    if (total_memory_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de memoria total\n");
        return;
    }

    // Creamos la métrica para las lecturas de disco SSD
    disk_reads_metric = prom_gauge_new("disk_reads_metric", "Lecturas de Disco Total", 0, NULL);
    if (disk_reads_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de lecturas de disco total\n");
        return;
    }

    loop_reads_metric = prom_gauge_new("loop_reads_metric", "Lecturas de Dispositivos de Montaje de archivos", 0, NULL);
    if (loop_reads_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de lecturas de dispositivos de montaje \n");
        return;
    }

    disk_writes_metric = prom_gauge_new("disk_writes_metric", "Escrituras de Disco Total", 0, NULL);
    if (disk_writes_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de escrituras de disco total\n");
        return;
    }

    loop_writes_metric = prom_gauge_new("loop_writes_metric", "Escrituras de Dispositivos de montaje", 0, NULL);
    if (loop_writes_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de escrituras de dispositivos de montaje\n");
        return;
    }

    time_reads_metric = prom_gauge_new("times_reads_metric", "Tiempos de lectura de dispositivos", 0, NULL);
    if (time_reads_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de tiempo de lectura de los dispositivos \n");
        return;
    }

    time_writes_metric = prom_gauge_new("times_writes_metric", "Tiempo de escritura de dispositivos", 0, NULL);
    if (time_writes_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de escritura de los dispositivos \n");
        return;
    }

    IO_in_progress_metric = prom_gauge_new("IO_in_progress_metric", "Cantidad de operaciones de E/S", 0, NULL);
    if (IO_in_progress_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de cantidad de operaciones de E/S \n");
        return;
    }

    time_IO_metric = prom_gauge_new("time_IO_metric", "Tiempo de operaciones de E/S", 0, NULL);
    if (time_IO_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica tiempo de operaciones de E/S \n");
        return;
    }

    num_processes_metric = prom_gauge_new("num_processes_metric", "Cantidad de procesos en ejecución", 0, NULL);
    if (num_processes_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de procesos en ejecución\n");
        return;
    }

    received_bytes_metric = prom_gauge_new("received_bytes_metric", "Cantidad de bytes recibidos", 0, NULL);
    if (received_bytes_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de bytes recibidos\n");
        return;
    }

    sent_bytes_metric = prom_gauge_new("sent_bytes_metric", "Cantidad de bytes enviados", 0, NULL);
    if (sent_bytes_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de bytes enviados\n");
        return;
    }

    received_packets_metric = prom_gauge_new("received_packets_metric", "Cantidad de paquetes recibidos", 0, NULL);
    if (received_packets_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de paquetes recibidos\n");
        return;
    }

    sent_packets_metric = prom_gauge_new("sent_packets_metric", "Cantidad de paquetes enviados", 0, NULL);
    if (sent_packets_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de paquetes enviados\n");
        return;
    }

    received_errors_metric = prom_gauge_new("received_errors_metric", "Cantidad de errores de recepción", 0, NULL);
    if (received_errors_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de errores de recepción\n");
        return;
    }

    sent_errors_metric = prom_gauge_new("sent_errors_metric", "Cantidad de errores de envío", 0, NULL);
    if (sent_errors_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de errores de envío\n");
        return;
    }

    user_time_metric = prom_gauge_new("user_time_metric", "Tiempo de CPU en modo usuario", 0, NULL);
    if (user_time_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de tiempo de CPU en modo usuario\n");
        return;
    }

    kernel_time_metric = prom_gauge_new("kernel_time_metric", "Tiempo de CPU en modo kernel", 0, NULL);
    if (kernel_time_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de tiempo de CPU en modo kernel\n");
        return;
    }

    inactive_time_metric = prom_gauge_new("inactive_time_metric", "Tiempo de inactividad de la CPU", 0, NULL);
    if (inactive_time_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de tiempo de inactividad de la CPU\n");
        return;
    }

    io_wait_metric = prom_gauge_new("io_wait_metric", "Tiempo de espera de entrada/salida de la CPU", 0, NULL);
    if (io_wait_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de tiempo de espera de I/O\n");
        return;
    }

    // Registramos las métricas en el registro por defecto
    bool reg_metricas = registroMetricas();
    if (reg_metricas)
    {
        fprintf(stderr, "Error al registrar las métricas\n");
        return;
    }
}

bool registroMetricas()
{
    if (prom_collector_registry_must_register_metric(cpu_usage_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(memory_usage_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(free_memory_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(used_memory_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(total_memory_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(disk_reads_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(loop_reads_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(disk_writes_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(loop_writes_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(time_reads_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(time_writes_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(IO_in_progress_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(time_IO_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(num_processes_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(received_bytes_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(sent_bytes_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(received_packets_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(sent_packets_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(received_errors_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(sent_errors_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(user_time_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(kernel_time_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(inactive_time_metric) == NULL)
        return true;
    if (prom_collector_registry_must_register_metric(io_wait_metric) == NULL)
        return true;

    return false;
}

void destroy_mutex()
{
    pthread_mutex_destroy(&lock);
}
