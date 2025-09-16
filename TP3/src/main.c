#include "memory.h"

#define MAX_BLOCKS 50
#define MAX_SIZE 256

int main(void) {
    printf("=== Memory Stress Test ===\n");

    srand((unsigned int)time(NULL));

    // Eleccion de politica
    malloc_control(FIRST_FIT);
    printf("Policy: First Fit\n");

    void* blocks[MAX_BLOCKS];
    size_t sizes[MAX_BLOCKS];

    // Asignacion de varios bloques aleatorios
    for (int i = 0; i < MAX_BLOCKS; i++) {
        // sizes[i] = (rand() % MAX_SIZE) + 1;
        sizes[i] = (size_t)(rand() % MAX_SIZE) + 1;
        blocks[i] = malloc(sizes[i]);
        if (!blocks[i]) {
            printf("malloc fallo en bloque %d\n", i);
            continue;
        }
        // Llenado de datos
        memset(blocks[i], i, sizes[i]);
    }

    // Liberacion de algunos bloques al azar
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (i % 3 == 0 && blocks[i]) {
            free(blocks[i]);
            blocks[i] = NULL;
        }
    }

    // Realloc de algunos bloques
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i]) {
            //size_t new_size = sizes[i] + (rand() % 64);
            size_t new_size = sizes[i] + (size_t)(rand() % 64);

            blocks[i] = realloc(blocks[i], new_size);
            sizes[i] = new_size;
            if (blocks[i])
                memset(blocks[i], i + 10, sizes[i]);
        }
    }

    // Llamadas a calloc
    void* zero_block = calloc(20, sizeof(int));
    if (zero_block) {
        printf("calloc 20 ints -> inicializados a cero\n");
    }

    // Mostrar heap check de algunos bloques
    for (int i = 0; i < MAX_BLOCKS; i+=10) {
        if (blocks[i]) {
            printf("\nHeap check block %d:\n", i);
            check_heap(blocks[i]);
        }
    }

    // Liberacion de todo
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i]) free(blocks[i]);
    }
    free(zero_block);

    printf("\n=== Estadísticas finales ===\n");
    memory_usage_stats();

    // Imprimir logs
    printf("\n=== Logs de operaciones ===\n");
    print_logs();
    free_logs();

    return 0;
}
