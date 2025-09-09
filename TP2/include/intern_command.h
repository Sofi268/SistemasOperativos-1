/**
 * @file intern_command.h
 * @brief Ejecucion de comandos internos de la shell.
 */

#ifndef INTERNCOMMAND_H
#define INTERNCOMMAND_H

#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief Longitud máxima para nombres de directorios.
 *
 * Esta macro define el tamaño máximo en caracteres para almacenar un nombre de directorio.
 */
#define MAX_DIR_LENGTH 256

/**
 * @brief Longitud máxima de salida para impresión en pantalla.
 *
 * Esta macro especifica el límite de caracteres que pueden imprimirse en una única operación.
 */
#define MAX_PRINT 1056

/**
 * @brief Cambia el directorio de trabajo actual.
 *
 * Actualiza el directorio actual del shell al especificado en el parámetro.
 *
 * @param path Ruta al nuevo directorio.
 */
void changeDirectory(char* path);

/**
 * @brief Limpia la pantalla del terminal.
 *
 * Ejecuta el comando equivalente a `clear` para borrar el contenido visible del terminal.
 */
void clearScreen(void);

/**
 * @brief Imprime un mensaje en pantalla.
 *
 * Procesa y muestra el texto pasado como argumento.
 *
 * @param arg Cadena de texto a imprimir.
 */
void echoCommand(char* arg);

/**
 * @brief Verifica si el monitor está en ejecución.
 *
 * Esta función indica si el proceso monitor se encuentra actualmente activo.
 *
 * @return true si el monitor está en ejecución, false en caso contrario.
 */
bool get_isRunning(void);

/**
 * @brief Obtiene el PID del proceso monitor en ejecución.
 *
 * Devuelve el identificador del proceso asociado al monitor si está en ejecución.
 *
 * @return El PID del proceso monitor si está activo, o -1 si no hay ningún proceso en ejecución.
 */
pid_t get_idRunning(void);

/**
 * @brief Inicia el monitoreo de métricas del sistema.
 *
 * Activa el monitoreo continuo de métricas como uso de CPU, memoria y discos.
 */
void start_monitor(void);

/**
 * @brief Detiene el monitoreo de métricas del sistema.
 *
 * Finaliza cualquier proceso relacionado con la captura de métricas en ejecución
 * al matar al proceso hijo que estaba corriendo el programa en segundo plano.
 */
void stop_monitor(void);

/**
 * @brief Muestra el estado del monitoreo.
 *
 * Presenta información sobre el programa metrics del Tp1.
 */
void status_monitor(void);

/**
 * @brief Compila y ejecuta el programa de monitoreo de métricas del sistema.
 *
 * Esta función realiza las siguientes operaciones:
 * - Ejecuta `make` en un submódulo para compilar el programa de métricas.
 * - Verifica si la compilación se completó correctamente.
 * - Inicia la ejecución del programa de monitoreo con el proceso hijo
 *   si la compilación fue exitosa.
 * - Almacena el PID del proceso del monitor y actualiza su estado.
 */
void run_metrics(void);

/**
 * @brief Proporciona interacción básica del shell y el programa de monitoreo.
 *
 * Habilita comandos básicos para interactuar con el monitor.
 */
void shell_interaction(void);

#endif // INTERNCOMMAND_H
