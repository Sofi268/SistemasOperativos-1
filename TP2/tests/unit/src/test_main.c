/**
 * @file test_main.c
 * @brief Entry point of tests
 */

#include "shell_test.h"

/**
 * @brief Configuración antes de cada prueba.
 */
void setUp(void)
{
}

/**
 * @brief Limpieza después de cada prueba.
 */
void tearDown(void)
{
}

/**
 * @brief Función principal del programa.
 */
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(get_Prompt_Test);
    RUN_TEST(test_changeDirectory);
    RUN_TEST(test_echoCommand);
    RUN_TEST(test_strip_quotes);
    RUN_TEST(test_clearScreen);
    RUN_TEST(test_changeSettings);
    return UNITY_END();
}
