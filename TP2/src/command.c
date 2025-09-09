#include "command.h"

void convert_subcommands_to_args(char* subcommands[], char* args[])
{
    int index = 0;

    for (int i = 0; subcommands[i] != NULL; i++)
    {
        char* token = strtok(subcommands[i], " "); // Divide el subcomando en tokens
        while (token != NULL)
        {
            if (strcmp(token, "|") != 0)
            { // Si el token no es '|', lo agrega al arreglo
                args[index++] = token;
            }
            else
            {
                args[index++] = NULL; // Marca el fin del subcomando
            }
            token = strtok(NULL, " "); // Obtener el siguiente token
        }
    }

    args[index] = NULL; // Marca el fin del arreglo completo
}

void execPipes(char* subcommands[], int num_pipes)
{
    char* a[MAX_ARGS];
    convert_subcommands_to_args(subcommands, a);

    int fds[2];
    int prev_fd = -1; // Para almacenar el descriptor de lectura previo
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    int cmd_start = 0; // Índice donde comienza cada comando

    for (int i = 0; i <= num_pipes; i++)
    {
        if (pipe(fds) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        { // Proceso hijo
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO); // Redirige la entrada desde el pipe anterior
                close(prev_fd);
            }
            if (i < num_pipes)
            {
                dup2(fds[1], STDOUT_FILENO); // Redirige la salida al pipe actual
            }

            // Cierra los extremos no utilizados
            close(fds[0]);
            close(fds[1]);

            // Ejecutar el comando actual
            execvp(a[cmd_start], &a[cmd_start]);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else
        {                  // Proceso padre
            close(fds[1]); // Cierra el extremo de escritura del pipe
            if (prev_fd != -1)
            {
                close(prev_fd); // Cierra el descriptor anterior ya procesado
            }

            prev_fd = fds[0]; // Almacena el extremo de lectura actual
            wait(NULL);       // Espera al proceso hijo
        }

        // Mover cmd_start al siguiente subcomando
        while (a[cmd_start] != NULL)
        {
            cmd_start++;
        }
        cmd_start++; // Saltar el NULL que separa los subcomandos
    }

    // Leer la salida del último pipe
    while ((bytesRead = read(prev_fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }
    close(prev_fd); // Cierra el último pipe
}

void execCommand(char* args[])
{
    if (args[0] == NULL)
    {
        printf("No se ingreso ningun comando");
        return;
    }

    // Comandos internos
    if (strcmp(args[0], "cd") == 0)
    {
        changeDirectory(args[1]);
    }
    else if (strcmp(args[0], "clr") == 0)
    {
        clearScreen();
    }
    else if (strcmp(args[0], "echo") == 0)
    {
        char echo_arg[MAX_ECHO_ARGS] = ""; // Buffer para almacenar todos los argumentos concatenados

        for (int i = 1; args[i] != NULL; i++)
        {
            strcat(echo_arg, args[i]);
            if (args[i + 1] != NULL)
            {
                strcat(echo_arg, " "); // Agregar un espacio entre los argumentos
            }
        }

        echoCommand(echo_arg); // Llamar a echoCommand con todos los argumentos concatenados
    }
    else
    {
        // Invocar programas externos
        externProgram(args);
    }
}

void execCommandWithRedirection(char* args[])
{
    int in_fd = -1, out_fd = -1; // Descriptores para redirección
    char* command[MAX_ARGS];
    int i = 0, j = 0;

    // Procesar los argumentos buscando redirección
    while (args[i] != NULL)
    {
        if (strcmp(args[i], "<") == 0)
        { // Redirección de entrada
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "Error: Archivo no especificado para redirección de entrada\n");
                return;
            }
            in_fd = open(args[i + 1], O_RDONLY);
            if (in_fd < 0)
            {
                perror("Error al abrir archivo de entrada");
                return;
            }
            i += 2; // Saltar operador y archivo
        }
        else if (strcmp(args[i], ">") == 0)
        { // Redirección de salida
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "Error: Archivo no especificado para redirección de salida\n");
                return;
            }
            out_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd < 0)
            {
                perror("Error al abrir archivo de salida");
                return;
            }
            i += 2; // Saltar operador y archivo
        }
        else
        {
            command[j++] = args[i++]; // Agregar argumento al comando
        }
    }

    command[j] = NULL; // Terminar el comando

    pid_t pid = fork();
    if (pid == 0)
    { // Proceso hijo
        if (in_fd != -1)
        {
            dup2(in_fd, STDIN_FILENO); // Redirigir entrada
            close(in_fd);
        }
        if (out_fd != -1)
        {
            dup2(out_fd, STDOUT_FILENO); // Redirigir salida
            close(out_fd);
        }
        execvp(command[0], command); // Ejecutar comando
        perror("Error al ejecutar comando");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    { // Proceso padre
        if (in_fd != -1)
            close(in_fd);
        if (out_fd != -1)
            close(out_fd);
        wait(NULL); // Esperar al hijo
    }
    else
    {
        perror("Error al crear proceso");
    }
}
