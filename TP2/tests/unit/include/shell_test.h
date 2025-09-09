/**
 * @file shell_test.h
 * @brief Pruebas del shell.
 *
 * Funciones de prueba para comprobar el comportamiento general del shell.
 */

#include "intern_command_test.h"
#include <shell.h>

/**
 * @brief  Máximo largo de pwc.
 *
 * Esta macro define el límite de longitud de pwc.
 */
#define MAX_LENGTH 512

/**
 * @brief Prueba la función getPrompt.
 *
 * Valida que el prompt se genere correctamente, incluyendo usuario, host y directorio actual.
 */
void get_Prompt_Test(void);

/**
 * @brief Prueba la función strip_quotes.
 *
 * Verifica que las comillas simples o dobles sean eliminadas correctamente de los argumentos.
 */
void test_strip_quotes(void);

/**
 * @brief Prueba la función changeSettings.
 *
 * Valida que los cambios en el archivo de configuración JSON se realicen correctamente.
 */
void test_changeSettings(void);
