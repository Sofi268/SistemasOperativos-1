/**
 * @file main.c
 * @brief Entry point of the system
 */

#include "expose_metrics.h"
#include <stdbool.h>

/**
 * @brief Tiempo de espera en segundos.
 *
 * Esta macro define el tiempo que el programa esperará entre ciertas operaciones.
 */
#define SLEEP_TIME 1

/**
 * @brief Función principal del programa.
 *
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Arreglo de cadenas que representan los argumentos.
 * @return int Código de salida del programa.
 */
int main(int argc, char* argv[])
{
    init_metrics();
    // Creamos un hilo para exponer las métricas vía HTTP
    pthread_t tid;
    if (pthread_create(&tid, NULL, expose_metrics, NULL) != 0)
    {
        fprintf(stderr, "Error al crear el hilo del servidor HTTP\n");
        return EXIT_FAILURE;
    }

    // Setea la memoria total una vez
    set_total_memory_gauge();

    // Bucle principal para actualizar las métricas cada segundo
    while (true)
    {
        update_cpu_gauge();
        update_memory_gauge();
        update_free_memory_gauge();
        update_used_memory_gauge();
        update_disk_reads();
        update_loop_reads();
        update_disk_writes();
        update_loop_writes();
        update_time_reads();
        update_time_writes();
        update_IO_in_progress();
        update_time_in_IO();
        update_num_processes();
        update_received_bytes();
        update_sent_bytes();
        update_received_packets();
        update_sent_packets();
        update_received_errors();
        update_sent_errors();
        update_user_time();
        update_kernel_time();
        update_inactive_time();
        update_IO_wait();

        sleep(SLEEP_TIME);
    }

    return EXIT_SUCCESS;
}
