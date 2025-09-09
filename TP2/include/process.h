/**
 * @file process.h
 * @brief Manejo de procesos en primer plano externos y su ejecución en la shell.
 */

#ifndef PROCESS_H
#define PROCESS_H

#include "signals.h"

/**
 * @brief Ejecuta un programa externo en primer plano.
 *
 * Crea un proceso hijo para ejecutar un programa que no sea un comando interno.
 * El proceso padre espera a que el hijo termine antes de continuar.
 *
 * @param args Comando y argumentos a ejecutar.
 */
void externProgram(char** args);

#endif // PROCESS_H
