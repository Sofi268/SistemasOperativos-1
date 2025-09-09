/**
 * @file metrics.h
 * @brief Funciones para obtener el uso de CPU y memoria desde el sistema de archivos /proc.
 */

#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Tamaño del buffer utilizado para almacenar datos.
 *
 * Esta macro define el tamaño máximo del buffer en bytes.
 */
#define BUFFER_SIZE 256

/**
 * @brief Cantidad de estadísticas de CPU a leer.
 *
 * Esta macro define el número de valores de uso de CPU que se leen
 * desde el archivo /proc/stat.
 */
#define CPU_STATS_COUNT 8

/**
 * @brief Tamaño del buffer para múltiples lecturas.
 *
 * Esta macro define el tamaño del buffer en bytes, multiplicado por 4,
 * para asegurar que haya suficiente espacio para almacenar múltiples líneas
 * o datos de uso de CPU.
 */
#define MULTI_BUFFER_SIZE (4 * BUFFER_SIZE)

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
 * @brief Obtiene la memoria libre desde /proc/meminfo.
 *
 * @return Memoria disponible o -1.0 en caso de error.
 */
double get_free_memory();

/**
 * @brief Obtiene la memoria libre desde /proc/meminfo.
 *
 * @return Memoria en uso, o -1.0 en caso de error.
 */
double get_used_memory();

/**
 * @brief Obtiene la memoria libre desde /proc/meminfo.
 *
 * @return Memoria total, o -1.0 en caso de error.
 */
double get_total_memory();

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
 * @brief Obtiene las lecturas totales del disco SSD desde /proc/diskstats
 *
 * Suma la cantidad de lecturas del disco SSD suponiendo que los dispositivos que lo leen comienzan por sda
 *
 * @return Cantidad total de lecturas completadas tanto por el disco duro como por el SSD
 */
double get_disk_reads();

/**
 * @brief Obtiene las lecturas totales de dispositivos loop desde /proc/diskstats
 *
 * Estos hacen referencia a dispositivos para el montaje de archivos como si fueran particiones de disco fisicas
 * Ej: imagenes de disco
 *
 * @return Cantidad total de lecturas de dispositivos loop
 */
double get_loop_reads();

/**
 * @brief Obtiene la cantidad total de escrituras en disco desde /proc/diskstats
 *
 * Suma las operaciones de escritura de todos los dispositivos
 *
 * @return Cantidad total de escrituras en disco
 */
double get_disk_writes();

/**
 * @brief Obtiene la cantidad total de escrituras para el montaje de archivos desde /proc/diskstats
 *
 * Suma las operaciones de escritura de todos los dispositivos que montan archivos
 *
 * @return Cantidad total de escrituras en el montaje de archivos
 */
double get_loop_writes();

/**
 * @brief Obtiene el tiempo total que demora la lectura de los dispositivos desde /proc/diskstats
 *
 * @return Los segundos que tardan los dispositivos en la lectura de archivos
 */
double get_time_reads();

/**
 * @brief Obtiene el tiempo total que demora la escritura de los dispositivos desde /proc/diskstats
 *
 * @return Los segundos que tardan los dispositivos en la escritura de dispositivos
 */
double get_time_writes();

/**
 * @brief Obtiene la cantidad total de operaciones de E/S en progreso desde desde /proc/diskstats
 *
 * @return el numero de operaciones de E/S
 */
double get_IO_in_progress();

/**
 * @brief Obtiene el tiempo total que los dispositivos han tomado para hacer operaciones de E/S desde /proc/diskstats
 *
 * @return devuelve el tiempo total de operaciones de E/S
 */
double get_time_in_IO();

/**
 * @brief Comprueba si una cadena consiste solo en dígitos
 *
 * @return devuelve 1 si solo son numeros y 0 si hay al menos un caracter
 */
int is_num(const char* process);

/**
 * @brief Obtiene la cantidad de procesos en ejecucion basandose en la cantidad de archivos con PID en el directorio
 * /proc
 *
 * @return devuelve la cantidad de procesos ejecutandose
 */
double get_num_processes();

/**
 * @brief Obtiene la cantidad de bytes recibidos por la interfaz de red.
 *
 * @return devuelve el total de bytes recibidos.
 */
double get_received_bytes();

/**
 * @brief Obtiene la cantidad de bytes enviados por la interfaz de red.
 *
 * @return devuelve el total de bytes enviados.
 */
double get_sent_bytes();

/**
 * @brief Obtiene la cantidad de paquetes recibidos por la interfaz de red.
 *
 * @return devuelve el total de paquetes recibidos.
 */
double get_received_packets();

/**
 * @brief Obtiene la cantidad de paquetes enviados por la interfaz de red.
 *
 * @return devuelve el total de paquetes enviados.
 */
double get_sent_packets();

/**
 * @brief Obtiene la cantidad de errores de recepción en la interfaz de red.
 *
 * @return devuelve el total de errores de recepción.
 */
double get_received_errors();

/**
 * @brief Obtiene la cantidad de errores de envío en la interfaz de red.
 *
 * @return devuelve el total de errores de envío.
 */
double get_sent_errors();

/**
 * @brief Obtiene el tiempo de CPU en modo usuario.
 *
 * @return Devuelve el total de tiempo en modo usuario.
 */
double get_user_time();

/**
 * @brief Obtiene el tiempo de CPU en modo kernel.
 *
 * @return Devuelve el total de tiempo en modo kernel.
 */
double get_kernel_time();

/**
 * @brief Obtiene el tiempo de inactividad de la CPU.
 *
 * @return Devuelve el total de tiempo inactivo.
 */
double get_inactive_time();

/**
 * @brief Obtiene el tiempo de espera de entrada/salida de la CPU.
 *
 * @return Devuelve el total de tiempo de espera por operaciones de I/O.
 */
double get_IO_wait();
