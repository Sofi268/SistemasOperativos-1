/**
 * @file memory.h
 * @brief Memory management library with custom allocation functions.
 *
 * Biblioteca que define funciones de asignacion de memoria dinamica
 * y gestion de bloques para crear un asignador de memoria personalizado.
 */

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief Macro para alinear una cantidad de bytes al siguiente multiplo de 8.
 *
 * @param x Cantidad de bytes a alinear.
 */
#define align(x) (((x) == 0) ? 8 : (((((x) - 1) >> 3) << 3) + 8))

/** Tamano minimo de un bloque de memoria. */
#define BLOCK_SIZE 40
/** Tamano de pagina en memoria. */
#define PAGESIZE 4096
/** Politica de asignacion First Fit. */
#define FIRST_FIT 0
/** Politica de asignacion Best Fit. */
#define BEST_FIT 1
/** Politica de asignacion Worst Fit. */
#define WORST_FIT 2
/** Tamano del bloque de inicio de datos. */
#define DATA_START 1

/**
 * @struct s_block
 * @brief Representa un bloque de memoria del heap.
 */
struct s_block
{
    size_t size;           /**< Tamano del bloque de datos. */
    struct s_block* next;  /**< Puntero al siguiente bloque en la lista enlazada. */
    struct s_block* prev;  /**< Puntero al bloque anterior en la lista enlazada. */
    int free;              /**< Indicador de si el bloque esta libre (1) o ocupado (0). */
    void* ptr;             /**< Puntero a la direccion de los datos almacenados. */
    char data[DATA_START]; /**< Area donde comienzan los datos del bloque. */
};

/**
 * @struct s_log_entry
 * @brief Entrada de registro para operaciones de memoria.
 */
typedef struct s_log_entry
{
    char op[10];              /**< "malloc", "free", "realloc", "calloc" */
    size_t size;              /**< Tamano de la operacion */
    time_t timestamp;         /**< Momento de la operacion */
    struct s_log_entry* next; /**< Puntero al siguiente registro */
    unsigned int counter;     /**< Numero de operacion consecutiva */
} t_log_entry;

/** Tipo de puntero para un bloque de memoria. */
typedef struct s_block* t_block;

/** Variables globales externas */
extern t_log_entry* log_head; /**< Puntero a la cabeza de los logs */
extern t_block base;          /**< Puntero al primer bloque de memoria */
extern int registro_malloc;   /**< Contador de llamadas a malloc */
extern int registro_free;     /**< Contador de llamadas a free */
extern int registro_calloc;   /**< Contador de llamadas a calloc */
extern int registro_realloc;  /**< Contador de llamadas a realloc */

/**
 * @brief Obtiene el bloque que contiene una direccion de memoria dada.
 *
 * @param p Puntero a la direccion de datos.
 * @return t_block Puntero al bloque de memoria correspondiente.
 */
t_block get_block(void* p);

/**
 * @brief Verifica si una direccion de memoria es valida.
 *
 * @param p Direccion de memoria a verificar.
 * @return int Retorna 1 si la direccion es valida, 0 en caso contrario.
 */
int valid_addr(void* p);

/**
 * @brief Encuentra un bloque libre que tenga al menos el tamano solicitado.
 *
 * @param last Puntero al ultimo bloque.
 * @param size Tamano solicitado.
 * @return t_block Puntero al bloque encontrado, o NULL si no se encuentra ninguno.
 */
t_block find_block(t_block* last, size_t size);

/**
 * @brief Expande el heap para crear un nuevo bloque de memoria.
 *
 * @param last Ultimo bloque del heap.
 * @param s Tamano del nuevo bloque.
 * @return t_block Puntero al nuevo bloque creado.
 */
t_block extend_heap(t_block last, size_t s);

/**
 * @brief Divide un bloque de memoria en dos, si el tamano solicitado es menor que el bloque disponible.
 *
 * @param b Bloque a dividir.
 * @param s Tamano del nuevo bloque.
 */
void split_block(t_block b, size_t s);

/**
 * @brief Fusiona un bloque libre con su siguiente bloque si tambien esta libre.
 *
 * @param b Bloque a fusionar.
 * @return t_block Puntero al bloque fusionado.
 */
t_block fusion(t_block b);

/**
 * @brief Copia el contenido de un bloque de origen a un bloque de destino.
 *
 * @param src Bloque de origen.
 * @param dst Bloque de destino.
 */
void copy_block(t_block src, t_block dst);

/**
 * @brief Asigna un bloque de memoria del tamano solicitado.
 *
 * @param size Tamano en bytes del bloque a asignar.
 * @return void* Puntero al area de datos asignada.
 */
void* malloc(size_t size);

/**
 * @brief Libera un bloque de memoria previamente asignado.
 *
 * @param p Puntero al area de datos a liberar.
 */
void free(void* p);

/**
 * @brief Asigna un bloque de memoria para un numero de elementos, inicializandolo a cero.
 *
 * @param number Numero de elementos.
 * @param size Tamano de cada elemento.
 * @return void* Puntero al area de datos asignada e inicializada.
 */
void* calloc(size_t number, size_t size);

/**
 * @brief Cambia el tamano de un bloque de memoria previamente asignado.
 *
 * @param p Puntero al area de datos a redimensionar.
 * @param size Nuevo tamano en bytes.
 * @return void* Puntero al area de datos redimensionada.
 */
void* realloc(void* p, size_t size);

/**
 * @brief Verifica el estado del heap y detecta bloques libres consecutivos.
 *
 * @param data Informacion adicional para la verificacion.
 */
void check_heap(void* data);

/**
 * @brief Configura el modo de asignacion de memoria.
 *
 * @param mode Modo de asignacion:
 *  - 0 -> First Fit
 *  - 1 -> Best Fit
 *  - 2 -> Worst Fit
 */
void malloc_control(int mode);

/**
 * @brief Establece directamente la politica de asignacion de memoria.
 *
 * @param m Metodo a usar (FIRST_FIT, BEST_FIT o WORST_FIT).
 */
void set_method(int m);

/**
 * @brief Imprime el uso actual de memoria (bloques ocupados y libres).
 */
void memory_usage();

/**
 * @brief Agrega una entrada de log para una operacion de memoria.
 *
 * @param op Tipo de operacion ("malloc", "free", "realloc", "calloc").
 * @param size Tamano asociado a la operacion (0 si no aplica).
 * @param counter Numero de operacion consecutiva.
 */
void add_log(const char* op, size_t size, unsigned int counter);

/**
 * @brief Imprime todas las entradas de log registradas.
 */
void print_logs();

/**
 * @brief Libera la memoria utilizada por las entradas de log.
 */
void free_logs();

/**
 * @brief Imprime estadisticas de uso de memoria (memoria usada, libre y fragmentacion).
 */
void memory_usage_stats();

