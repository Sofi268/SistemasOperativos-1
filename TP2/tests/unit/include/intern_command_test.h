/**
 * @file intern_command_test.h
 * @brief Pruebas de comandos internos de la shell.
 *
 * Funciones de prueba para verificar comandos internos.
 */

#include <assert.h>
#include <intern_command.h>
#include <unity.h>

/**
 * @brief  Tiempo que se espera a que se active el monitor.
 *
 */
#define SLEEP_T 5

/**
 * @brief Prueba la función changeDirectory.
 *
 * Verifica que cambie correctamente al directorio especificado y maneje errores.
 */
void test_changeDirectory(void);

/**
 * @brief Prueba la función clearScreen.
 *
 * Comprueba que limpie correctamente el contenido del terminal.
 */
void test_clearScreen(void);

/**
 * @brief Prueba la función echoCommand.
 *
 * Verifica que reproduzca correctamente el texto ingresado en la salida estándar.
 */
void test_echoCommand(void);
