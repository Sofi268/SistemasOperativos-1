/**
 * @file main.c
 * @brief Entry point of the system
 */

#include "expose_metrics.h"

/**
 * @brief Tiempo de espera en segundos.
 *
 * Esta macro define el tiempo que el programa esperará entre ciertas operaciones.
 */
#define SLEEP_TIME 1

#define NUM_PTRS 50
#define MAX_SIZE 1024

static void *ptrs[NUM_PTRS] = {0};
static size_t sizes[NUM_PTRS] = {0};

static inline unsigned long long diff_ns(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000000000ULL +
           (end.tv_nsec - start.tv_nsec);
}

/**
 * @brief Función principal del programa.
 *
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Arreglo de cadenas que representan los argumentos.
 * @return int Código de salida del programa.
 */
int main(void) {
    int i = 0;
    init_metrics();
    pthread_t tid;
    if (pthread_create(&tid, NULL, expose_metrics, NULL) != 0)
    {
        fprintf(stderr, "Error al crear el hilo del servidor HTTP\n");
        return EXIT_FAILURE;
    }

    while (i<=10) {
        int op = rand() % 4;  // 0=malloc, 1=free, 2=realloc, 3=calloc
        int idx = rand() % NUM_PTRS;
        size_t sz = (rand() % MAX_SIZE) + 32;

        struct timespec start, end;
        void *tmp;

        switch (op) {
            case 0: // malloc
                clock_gettime(CLOCK_MONOTONIC, &start);
                tmp = new_malloc(sz);
                clock_gettime(CLOCK_MONOTONIC, &end);
                if (ptrs[idx]) new_free(ptrs[idx]);
                ptrs[idx] = tmp;
                sizes[idx] = sz;
                update_avg_malloc_time(diff_ns(start, end));
                break;

            case 1: // free
                if (ptrs[idx]) {
                    clock_gettime(CLOCK_MONOTONIC, &start);
                    new_free(ptrs[idx]);
                    clock_gettime(CLOCK_MONOTONIC, &end);
                    ptrs[idx] = NULL;
                    sizes[idx] = 0;
                    update_avg_free_time(diff_ns(start, end));
                }
                break;

            case 2: // realloc
                if (ptrs[idx]) {
                    clock_gettime(CLOCK_MONOTONIC, &start);
                    tmp = new_realloc(ptrs[idx], sz);
                    clock_gettime(CLOCK_MONOTONIC, &end);
                    if (tmp) {
                        ptrs[idx] = tmp;
                        sizes[idx] = sz;
                    }
                    update_avg_realloc_time(diff_ns(start, end));
                }
                break;

            case 3: // calloc
                clock_gettime(CLOCK_MONOTONIC, &start);
                tmp = new_calloc(1, sz);
                clock_gettime(CLOCK_MONOTONIC, &end);
                if (ptrs[idx]) new_free(ptrs[idx]);
                ptrs[idx] = tmp;
                sizes[idx] = sz;
                update_avg_calloc_time(diff_ns(start, end));
                break;
        }
        update_metrics();
        i++;
        sleep(SLEEP_TIME);
    }
}
