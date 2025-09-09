#include "process.h"

void externProgram(char** args)
{
    pid_t pid = fork(); // Creación del proceso hijo

    if (pid < 0)
    {
        // Error al crear el proceso
        perror("Error al crear el proceso");
        return;
    }

    if (pid == 0)
    {
        // Proceso hijo
        setup_signal_handlers();
        printf("\n");
        if (execvp(args[0], args) == -1)
        {
            perror("Error al ejecutar el programa");
            exit(EXIT_FAILURE);
        }
    }

    else
    {
        // Proceso padre
        int child_status;
        waitpid(pid, &child_status, 0); // Esperar a que termine el hijo
    }
}
