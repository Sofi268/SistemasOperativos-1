#include "jobs.h"

void backgroundProcess(char** args, int jobID)
{
    pid_t pid = fork(); // Creación de un nuevo proceso

    if (pid < 0)
    {
        // Error al crear el proceso
        perror("Error al crear el proceso en segundo plano");
        return;
    }

    if (pid == 0)
    {
        // Proceso hijo
        printf("\n");
        if (execvp(args[0], args) == -1)
        {
            perror("Error al ejecutar el programa en segundo plano");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Proceso padre
        printf("[%d] %d \n", jobID, pid);
    }
}
