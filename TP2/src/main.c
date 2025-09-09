/**
 * @file main.c
 * @brief Entry point of the system
 */

#include <stdio.h>
#include <stdlib.h>

#include "shell.h"

/**
 * @brief Tiempo que deja correr al monitor con los nuevos parametros.
 */
#define SLEEP_TIME 15

/**
 * @brief Función principal del programa.
 *
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Arreglo de cadenas que representan los argumentos.
 * @return int Código de salida del programa.
 */
int main(int argc, char* argv[])
{
    runShell(argc, argv);

    return EXIT_SUCCESS;
}
