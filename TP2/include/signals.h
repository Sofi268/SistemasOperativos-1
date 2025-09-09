/**
 * @file signals.h
 * @brief Manejo de señales en la shell.
 */
#ifndef SIGNALS_H
#define SIGNALS_H

#include "intern_command.h"

/**
 * @brief Manejador de la señal SIGINT.
 *
 * Esta función maneja la señal SIGINT (Ctrl+C) y envía la misma señal al proceso en primer plano.
 *
 * @param sig La señal recibida (no se utiliza en esta implementación).
 */
void sigint_handler(int sig);

/**
 * @brief Manejador de la señal SIGTSTP.
 *
 * Esta función maneja la señal SIGTSTP (Ctrl+Z) y envía la misma señal al proceso en primer plano.
 *
 * @param sig La señal recibida (no se utiliza en esta implementación).
 */
void sigtstp_handler(int sig);

/**
 * @brief Manejador de la señal SIGQUIT.
 *
 * Esta función maneja la señal SIGQUIT (Ctrl+\) y envía la misma señal al proceso en primer plano.
 *
 * @param sig La señal recibida (no se utiliza en esta implementación).
 */
void sigquit_handler(int sig);

/**
 * @brief Configura los manejadores de señales.
 *
 * Esta función configura los manejadores de las señales SIGINT, SIGTSTP y SIGQUIT.
 * Asocia cada señal a su respectivo manejador.
 */
void setup_signal_handlers();

#endif // SIGNALS_H
