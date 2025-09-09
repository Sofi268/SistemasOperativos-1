/**
 * @file shell.h
 * @brief Manejo de la shell y su interacción con el usuario.
 */
#ifndef SHELL_H
#define SHELL_H

#include <pthread.h>

#include "command.h"
#include "jobs.h"

#include <cjson/cJSON.h>

/**
 * @brief Longitud máxima para un comando.
 *
 * Esta macro define el límite de caracteres que un comando puede tener.
 */
#define MAX_CMD_LENGTH 512

/**
 * @brief Tamaño máximo para el prompt.
 *
 * Esta macro define el tamaño máximo para la cadena que representa el prompt.
 */
#define MAX_PROMPT 1056

/**
 * @brief Longitud máxima para el nombre del host.
 *
 * Esta macro define el tamaño máximo del nombre del host.
 */
#define MAX_HOST_LENGTH 256

/**
 * @brief Ejecuta un archivo de comandos en modo batch.
 *
 * Abre y lee el archivo de comandos línea por línea, ejecutando cada comando de forma secuencial.
 * Si encuentra un comando para ejecutarse en segundo plano, lo maneja adecuadamente.
 *
 * @param filename Nombre del archivo de comandos a ejecutar.
 */
void executeBatchFile(const char* filename);

/**
 * @brief Obtiene el prompt de la shell.
 *
 * Construye un prompt que incluye el nombre de usuario, el nombre del host y el directorio actual.
 *
 * @return Una cadena con el prompt formateado.
 */
char* getPrompt();

/**
 * @brief Elimina las comillas dobles o simples de un argumento para que se puedan procesar archivos escritos tanto con
 * comillas como sin.
 *
 * Si el argumento comienza y termina con comillas, las elimina.
 *
 * @param arg Argumento a procesar.
 */
void strip_quotes(char* arg);

/**
 * @brief Ejecuta la shell interactiva.
 *
 * Si se proporciona un archivo como argumento, ejecuta el archivo en modo batch.
 * De lo contrario, espera comandos interactivos del usuario y los ejecuta.
 * Determina y llama a los metodos correspondientes dependiendo de que tipo de coando se tratase:
 * - Con redireccion ('<' o '>')
 * - Con pipes
 * - En segundo plano
 * - Comandos internos
 * - Comando en primer plano
 *
 * @param argc Número de argumentos.
 * @param argv Arreglo de argumentos, donde el primer argumento es el archivo de comandos.
 */
void runShell(int argc, char* argv[]);

/**
 * @brief Ejecuta un comando en segundo plano.
 *
 * Asigna un ID de trabajo y llama a `backgroundProcess` para ejecutar el
 * comando en segundo plano.
 *
 * @param args Arreglo de argumentos del comando.
 * @param i Número de argumentos del comando.
 */
void runBackground(char* args[MAX_ARGS], int i);

/**
 * @brief Modifica las configuraciones del programa de monitoreo.
 *
 * Actualiza el archivo JSON de configuración con nuevos parametros.
 *
 * @param new_interval Nuevo intervalo de muestreo en segundos.
 * @param new_metrics Arreglo de cadenas que representa las nuevas métricas
 *                    a exponer.
 * @param metrics_count Número de métricas en el arreglo `new_metrics`.
 * @param memTot bool si se quiere incluir.
 * @param filename es el path sobre el cual se van a realizar los cambios, en este caso el config.json
 */
void changeSettings(int new_interval, const char* new_metrics[], int metrics_count, int memTot, const char* filename);

/**
 * @brief Genera un arreglo con configuraciones del archivo `config.json`.
 *
 * Lee y organiza las configuraciones del programa en un arreglo dinámico
 * de cadenas para su uso como parámetros del monitoreo.
 *
 * @return Arreglo dinámico de configuraciones (`char **args`), terminado en `NULL`.
 */
char** updateMetrics();

/**
 * @brief Actualiza la configuración del monitoreo y ejecuta el programa de métricas.
 *
 * Lee las configuraciones desde `config.json`, ejecuta `make` en el submódulo
 * y luego corre el programa de métricas en segundo plano, pasándole las configuraciones
 * como parámetros.
 *
 * @note No espera a que el programa de métricas termine, lo deja corriendo en segundo plano.
 */
void actualizarMonitor();

/**
 * @brief Detiene al monitor.
 */
void stopMonitor();

#endif // SHELL_H
