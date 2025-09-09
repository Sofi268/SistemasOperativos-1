/**
 * @file jobs.h
 * @brief Manejo de trabajos en segundo plano y mantencion de su estado.
 */

#ifndef JOBS_H
#define JOBS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * @brief Ejecuta un comando en segundo plano.
 *
 * Crea un proceso hijo para ejecutar el comando especificado,
 * permitiendo que el shell continúe funcionando sin esperar su finalización.
 *
 * @param args Comando y argumentos a ejecutar, terminados en `NULL`.
 * @param jobID Identificador del trabajo en segundo plano.
 */
void backgroundProcess(char** args, int jobID);

#endif // JOBS_H
