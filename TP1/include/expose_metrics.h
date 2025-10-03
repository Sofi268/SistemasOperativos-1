/**
 * @file expose_metrics.h
 * @brief Programa para leer el uso de CPU y memoria y exponerlos como métricas de Prometheus.
 */

#include "metrics.h"

/**
 * @brief Tamaño del buffer utilizado para almacenar datos.
 *
 * Esta macro define el tamaño máximo del buffer en bytes.
 */
#define BUFFER_SIZE 256

/**
 * @brief Tiempo de espera en segundos.
 *
 * Esta macro define el tiempo que el programa esperará entre ciertas operaciones.
 */
#define SLEEP_TIME 1

/**
 * @brief Actualiza la métrica de uso de CPU.
 */
void update_cpu_gauge();

/**
 * @brief Actualiza la métrica de uso de memoria.
 */
void update_memory_gauge();

/**
 * @brief Actualiza la métrica de memoria disponible.
 */
void update_free_memory_gauge();

/**
 * @brief Actualiza la métrica de memoria en uso.
 */
void update_used_memory_gauge();

/**
 * @brief Establece la métrica de memoria en uso.
 */
void set_total_memory_gauge();

/**
 * @brief Actualiza la métrica de lecturas de disco SSD.
 */
void update_disk_reads();

/**
 * @brief Actualiza la métrica de lecturas de dispositivos de montaje de archivos.
 */
void update_loop_reads();

/**
 * @brief Actualiza la métrica de escrituras de disco SSD.
 */
void update_disk_writes();

/**
 * @brief Actualiza la métrica de lecturas de dispositivos de montaje de archivos.
 */
void update_loop_writes();

/**
 * @brief Actualiza la métrica de tiempo de lecturas totales.
 */
void update_time_reads();

/**
 * @brief Actualiza la métrica de tiempo de escritura total.
 */
void update_time_writes();

/**
 * @brief Actualiza la métrica de cantidad de operaciones de E/S.
 */
void update_IO_in_progress();

/**
 * @brief Actualiza la métrica de tiempo de operaciones de E/S.
 */
void update_time_in_IO();

/**
 * @brief Actualiza la métrica de tiempo de operaciones de E/S.
 */
void update_num_processes();

/**
 * @brief Actualiza la métrica de bytes recibidos.
 */
void update_received_bytes();

/**
 * @brief Actualiza la métrica de bytes enviados.
 */
void update_sent_bytes();

/**
 * @brief Actualiza la métrica de paquetes recibidos.
 */
void update_received_packets();

/**
 * @brief Actualiza la métrica de paquetes enviados.
 */
void update_sent_packets();

/**
 * @brief Actualiza la métrica de errores de recepción.
 */
void update_received_errors();

/**
 * @brief Actualiza la métrica de errores de envío.
 */
void update_sent_errors();

/**
 * @brief Actualiza la métrica de tiempo de CPU en modo usuario.
 */
void update_user_time();

/**
 * @brief Actualiza la métrica de tiempo de CPU en modo kernel.
 */
void update_kernel_time();

/**
 * @brief Actualiza la métrica de tiempo de inactividad de la CPU.
 */
void update_inactive_time();

/**
 * @brief Actualiza la métrica de tiempo de espera de entrada/salida de la CPU.
 */
void update_IO_wait();

/**
 * @brief Actualiza la métrica tasa de ffragmentacion.
 */
void update_fragmentation_rate();

/**
 * @brief Actualiza la métrica de memoria RAM residente (RSS).
 */
void update_resident_memory();

/**
 * @brief Actualiza la métrica de memoria virtual utilizada por el proceso.
 */
void update_virtual_memory();

/**
 * @brief Actualiza la métrica de tamaño del heap.
 */
void update_heap_size();

/**
 * @brief Actualiza la métrica del tiempo promedio de malloc().
 *
 * @param elapsed_time Tiempo de la última ejecución en nanosegundos.
 */
void update_avg_malloc_time(unsigned long long elapsed_time);

/**
 * @brief Actualiza la métrica del tiempo promedio de free().
 *
 * @param elapsed_time Tiempo de la última ejecución en nanosegundos.
 */
void update_avg_free_time(unsigned long long elapsed_time);

/**
 * @brief Actualiza la métrica del tiempo promedio de realloc().
 *
 * @param elapsed_time Tiempo de la última ejecución en nanosegundos.
 */
void update_avg_realloc_time(unsigned long long elapsed_time);

/**
 * @brief Actualiza la métrica del tiempo promedio de calloc().
 *
 * @param elapsed_time Tiempo de la última ejecución en nanosegundos.
 */
void update_avg_calloc_time(unsigned long long elapsed_time);

/**
 * @brief Función del hilo para exponer las métricas vía HTTP en el puerto 8000.
 * @param arg Argumento no utilizado.
 * @return NULL
 */
void* expose_metrics(void* arg);

/**
 * @brief Inicializar mutex y métricas.
 */

void init_metrics();

/**
 * @brief Asegura que no haya error en el registro de las metricas
 */
bool registroMetricas();

/**
 * @brief Destructor de mutex
 */
void destroy_mutex();

/**
 * @brief Actualiza todas las metricas 
 */
void update_metrics();