/**
 * @file command.h
 * @brief Ejecucion de comandos, redireccion de salida y manejo de pipes.
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <fcntl.h>

#include "process.h"

/**
 * @brief  Máximo número de argumentos para un comando.
 *
 * Esta macro define el límite de argumentos que puede aceptar un comando.
 */
#define MAX_ARGS 100

/**
 * @brief  Máximo número de argumentos para el comando echo.
 *
 * Esta macro especifica el límite de caracteres que puede procesar el comando echo.
 */
#define MAX_ECHO_ARGS 1024

/**
 * @brief  Tamaño del buffer de lectura/escritura.
 *
 * Esta macro indica el tamaño máximo en bytes para operaciones de lectura y escritura.
 */
#define BUFFER_SIZE 1024

/**
 * @brief Convierte subcomandos en una lista de argumentos.
 *
 * Toma una lista de subcomandos separados por tuberías ("|") y los transforma
 * en un arreglo de argumentos para su ejecución.
 *
 * @param subcommands Array de cadenas que representan los subcomandos.
 * @param args Array donde se almacenarán los argumentos convertidos.
 */
void convert_subcommands_to_args(char* subcommands[], char* args[]);

/**
 * @brief Ejecuta una serie de subcomandos conectados por tuberías.
 *
 * Gestiona la creación de pipes y la ejecución secuencial de comandos.
 *
 * @param subcommands Array de subcomandos separados por "|".
 * @param pipe_count Número de pipes necesarios (cantidad de subcomandos - 1).
 */
void execPipes(char* subcommands[], int pipe_count);

/**
 * @brief Ejecuta un comando individual.
 *
 * Maneja tanto comandos internos como externos y realiza la ejecución correspondiente.
 *
 * @param args Array de cadenas que representan el comando y sus argumentos.
 */
void execCommand(char* args[]);

/**
 * @brief Ejecuta un comando con redirección de entrada/salida.
 *
 * Procesa las redirecciones especificadas en el comando y ajusta los descriptores
 * de archivo antes de ejecutar el comando.
 *
 * @param args Array de cadenas que representan el comando, argumentos y operadores
 * de redirección ("<", ">").
 */
void execCommandWithRedirection(char* args[]);

#endif // COMMAND_H
