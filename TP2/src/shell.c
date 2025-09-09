#include "shell.h"

pthread_mutex_t jobIDMutex = PTHREAD_MUTEX_INITIALIZER;

int jobID = 0;

void runBackground(char* args[MAX_ARGS], int i)
{
    args[i - 1] = NULL;

    // Proteccion de recurso compartido
    pthread_mutex_lock(&jobIDMutex);
    jobID++;
    int currentJobID = jobID;
    pthread_mutex_unlock(&jobIDMutex);

    backgroundProcess(args, currentJobID);
}

void executeBatchFile(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("No se pudo abrir el archivo");
        return;
    }

    char command[MAX_CMD_LENGTH];
    while (fgets(command, sizeof(command), file) != NULL)
    {
        command[strcspn(command, "\n")] = 0; // Eliminar el salto de línea

        // Separar el comando en argumentos
        char* args[MAX_ARGS];
        char* token = strtok(command, " ");
        int i = 0;

        while (token != NULL && i < MAX_ARGS - 1)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Terminar el array de argumentos

        // Verificar si el último argumento es '&' para ejecuctar en segundo plano
        if (i > 0 && strcmp(args[i - 1], "&") == 0)
        {
            runBackground(args, i);
        }

        // Para programas externos en primer plano
        else
        {
            externProgram(args);
        }
    }

    fclose(file);
}

char* getPrompt()
{
    static char dir[MAX_PROMPT];
    char hostname[MAX_HOST_LENGTH];

    // Obtener el nombre del host
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        perror("Error al obtener el nombre del host");
        return NULL;
    }

    // Obtener el nombre de usuario
    char* user_name = getenv("USER");
    // Obtener el directorio actual
    char* directory_name = getenv("PWD");
    // Obtener el directorio del usuario
    char* home_dir = getenv("HOME");

    if (!user_name || !directory_name || !home_dir)
    {
        fprintf(stderr, "Error: No se pudo obtener USER, PWD o HOME\n");
        return NULL;
    }

    // Reemplazar el directorio personal con ~ si corresponde
    char modified_dir[MAX_PROMPT];
    if (strncmp(directory_name, home_dir, strlen(home_dir)) == 0)
    {
        snprintf(modified_dir, sizeof(modified_dir), "~%s", directory_name + strlen(home_dir));
    }
    else
    {
        snprintf(modified_dir, sizeof(modified_dir), "%s", directory_name);
    }

    // Formatear el prompt con el signo $
    if (snprintf(dir, sizeof(dir), "%s@%s:%s$ ", user_name, hostname, modified_dir) < 0)
    {
        fprintf(stderr, "Error al formatear el prompt\n");
        return NULL;
    }

    return dir;
}

void strip_quotes(char* arg)
{
    size_t len = strlen(arg);
    if (len > 1 && ((arg[0] == '"' && arg[len - 1] == '"') || (arg[0] == '\'' && arg[len - 1] == '\'')))
    {
        // Si el argumento comienza y termina con la misma comilla, las elimina
        memmove(arg, arg + 1, len - 2);
        arg[len - 2] = '\0';
    }
}

void runShell(int argc, char* argv[])
{
    char command[MAX_CMD_LENGTH];

    if (argc > 1)
    {
        executeBatchFile(argv[1]);
    }
    else
    {
        while (1)
        {
            char* prompt = getPrompt();
            printf("%s ", prompt);
            fflush(stdout);
            bool redir = false;

            if (fgets(command, sizeof(command), stdin) == NULL)
            {
                perror("Error al leer el comando");
                continue;
            }

            command[strcspn(command, "\n")] = 0; // Eliminar el salto de línea

            if (strcmp(command, "quit") == 0)
            {
                exit(0);
            }

            char* args[MAX_ARGS] = {0};
            char* token = strtok(command, " ");
            int i = 0;
            int pipe_count = 0;

            while (token != NULL && i < MAX_ARGS - 1)
            {
                strip_quotes(token); // Eliminar las comillas si las tiene
                if (strcmp(token, "<") == 0 || strcmp(token, ">") == 0)
                {
                    redir = true;
                }

                if (strcmp(token, "|") == 0)
                {
                    // Validar que el pipe no sea redundante
                    if (i == 0 || (i > 0 && args[i - 1] == NULL))
                    {
                        fprintf(stderr, "Error: Pipe mal formado\n");
                        i = 0; // Resetea el comando
                        break;
                    }
                    pipe_count++;
                }
                args[i++] = token;
                token = strtok(NULL, " ");
            }

            if (i == 0)
            {
                continue; // Comando vacío o inválido
            }

            args[i] = NULL; // Termina el arreglo de argumentos

            if (redir)
            {
                execCommandWithRedirection(args);
            }
            else
            {
                // Controla si hay pipes
                if (pipe_count > 0)
                {
                    execPipes(args, pipe_count);
                    continue;
                }
                // Controla si es un proceso en segundo plano
                else if (i > 0 && strcmp(args[i - 1], "&") == 0)
                {
                    args[i - 1] = NULL; // Elimina '&' del arreglo de argumentos
                    runBackground(args, i);
                }
                // Ejecuta proceso en primer plano
                else
                {
                    execCommand(args);
                }
            }
        }
    }
}

void changeSettings(int new_interval, const char* new_metrics[], int metrics_count, int new_memtotal,
                    const char* filename)
{

    // Leer el archivo JSON existente
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        perror("Error abriendo el archivo config.json");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Asignar memoria para el contenido del archivo
    char* json_content = malloc(file_size + 1);
    if (!json_content)
    {
        perror("Error al asignar memoria para el contenido JSON");
        fclose(file);
        return;
    }

    // Leer el archivo en el buffer
    size_t bytes_read = fread(json_content, 1, file_size, file);
    if (bytes_read != file_size)
    {
        fprintf(stderr, "Error: No se pudo leer completamente el archivo config.json\n");
        fclose(file);
        free(json_content);
        return;
    }

    fclose(file);
    json_content[file_size] = '\0'; // Asegurarse de que el contenido del archivo termine con null

    // Parsear el contenido del JSON
    cJSON* config = cJSON_Parse(json_content);
    free(json_content); // Liberar la memoria del contenido leído
    if (!config)
    {
        fprintf(stderr, "Error al parsear config.json\n");
        return;
    }

    // Modificar el intervalo de muestreo
    cJSON* interval = cJSON_GetObjectItem(config, "sampling_interval");
    if (interval)
    {
        cJSON_SetIntValue(interval, new_interval);
    }
    else
    {
        cJSON_AddNumberToObject(config, "sampling_interval", new_interval);
    }

    // Modificar las métricas expuestas
    cJSON* metrics = cJSON_GetObjectItem(config, "exposed_metrics");
    if (metrics)
    {
        cJSON_DeleteItemFromObject(config, "exposed_metrics");
    }
    metrics = cJSON_AddArrayToObject(config, "exposed_metrics");

    // Agregar las nuevas métricas
    for (int i = 0; i < metrics_count; i++)
    {
        if (new_metrics[i] != NULL && strlen(new_metrics[i]) > 0)
        {
            cJSON_AddItemToArray(metrics, cJSON_CreateString(new_metrics[i]));
        }
        else
        {
            fprintf(stderr, "Error: Métrica inválida en posición %d\n", i);
        }
    }

    // Modificar el valor de MemTotal
    cJSON* memtotal = cJSON_GetObjectItem(config, "MemTotal");
    if (memtotal)
    {
        cJSON_SetIntValue(memtotal, new_memtotal);
    }
    else
    {
        cJSON_AddNumberToObject(config, "MemTotal", new_memtotal);
    }

    // Convertir el JSON actualizado a una cadena
    char* updated_json = cJSON_Print(config);
    if (!updated_json)
    {
        fprintf(stderr, "Error al imprimir el JSON actualizado\n");
        cJSON_Delete(config);
        return;
    }

    // Guardar el JSON actualizado en el archivo
    file = fopen(filename, "w+");
    if (!file)
    {
        perror("Error al abrir config.json para escritura");
        cJSON_Delete(config);
        free(updated_json);
        return;
    }

    fwrite(updated_json, 1, strlen(updated_json), file);
    fclose(file);

    // Imprimir el JSON actualizado en la consola
    printf("Configuración actualizada:\n%s\n", updated_json);

    // Liberar recursos
    cJSON_Delete(config);
    free(updated_json);
}

char** updateMetrics()
{
    // Abrir el archivo JSON
    FILE* file = fopen("../config.json", "r");
    if (!file)
    {
        perror("No se pudo abrir el archivo config.json");
        exit(EXIT_FAILURE);
    }

    // Leer el contenido del archivo
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* json_content = malloc(file_size + 1);
    if (!json_content)
    {
        perror("Error al asignar memoria para el contenido JSON");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    size_t bytes_read = fread(json_content, 1, file_size, file);
    if (bytes_read != file_size)
    {
        fprintf(stderr, "Error: No se pudo leer completamente el archivo JSON\n");
        fclose(file);
        free(json_content);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    json_content[file_size] = '\0';

    // Parsear el contenido JSON
    cJSON* root = cJSON_Parse(json_content);
    free(json_content);

    if (!root)
    {
        fprintf(stderr, "Error al parsear el archivo JSON\n");
        exit(EXIT_FAILURE);
    }

    // Obtener "sampling_interval"
    cJSON* sampling_interval_json = cJSON_GetObjectItem(root, "sampling_interval");
    if (!cJSON_IsNumber(sampling_interval_json))
    {
        fprintf(stderr, "No se encontró un número válido en 'sampling_interval'\n");
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    int sampling_interval = sampling_interval_json->valueint;

    // Obtener "MemTotal"
    cJSON* memtotal_json = cJSON_GetObjectItem(root, "MemTotal");
    if (!cJSON_IsNumber(memtotal_json))
    {
        fprintf(stderr, "No se encontró un número válido en 'MemTotal'\n");
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    int memtotal = memtotal_json->valueint;

    // Obtener "exposed_metrics"
    cJSON* metrics_json = cJSON_GetObjectItem(root, "exposed_metrics");
    if (!cJSON_IsArray(metrics_json))
    {
        fprintf(stderr, "No se encontró un arreglo válido en 'exposed_metrics'\n");
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }

    size_t metrics_count = cJSON_GetArraySize(metrics_json);

    // Armar el arreglo args[]
    int inicio_metric = 3; // 3 primeros son sampling_interval, MemTotal y log_level
    int total_args = inicio_metric + metrics_count;
    char** args = malloc(sizeof(char*) * (total_args + 1)); // +1 para el NULL final

    args[0] = strdup("log_level_value"); // Ejemplo de valor inicial para log_level

    args[1] = malloc(20 * sizeof(char)); // Para "sampling_interval"
    snprintf(args[1], 20, "%d", sampling_interval);

    args[2] = malloc(20 * sizeof(char)); // Para "MemTotal"
    snprintf(args[2], 20, "%d", memtotal);

    // Asignar métricas al arreglo args[]
    for (size_t i = 0; i < metrics_count; i++)
    {
        cJSON* metric_json = cJSON_GetArrayItem(metrics_json, i);
        if (cJSON_IsString(metric_json))
        {
            args[inicio_metric + i] = strdup(metric_json->valuestring);
        }
        else
        {
            fprintf(stderr, "Métrica inválida en la posición %zu\n", i);
            args[inicio_metric + i] = NULL;
        }
    }

    args[total_args] = NULL; // Terminar con NULL

    // Liberar el objeto JSON
    cJSON_Delete(root);

    return args; // Retornar el arreglo
}

void actualizarMonitor()
{
    char* args[MAX_ARGS];
    char** dynamicArgs = updateMetrics();
    int large = 0;

    // Copiar los valores del puntero dinámico al arreglo estático
    for (int i = 0; i < MAX_ARGS && dynamicArgs[i] != NULL; i++)
    {
        args[i] = dynamicArgs[i];
        large++;
    }
    args[large] = NULL; // Asegurar el NULL al final de makeargs[]

    // Ejecutar "make" en el submódulo
    pid_t makeID = fork();

    if (makeID == 0)
    { // Proceso hijo para make
        char* makeArgs[] = {"make", "-C", "../submodulos/Tp1-SO1", NULL};
        execvp(makeArgs[0], makeArgs);
        perror("Error al ejecutar make");
        exit(1);
    }
    else if (makeID > 0)
    { // Proceso padre
        int status;
        waitpid(makeID, &status, 0); // Esperar a que termine el "make"
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            printf("Compilación completada correctamente\n");
        }
        else
        {
            printf("Error durante la compilación\n");
            return;
        }
    }
    else
    {
        perror("Error al crear el proceso para make");
        return;
    }

    // Ejecutar el programa de métricas con los argumentos en segundo plano
    pid_t monitorID = fork();

    if (monitorID == 0)
    { // Proceso hijo para metrics
        execvp("../submodulos/Tp1-SO1/metrics", args);
        perror("Error al ejecutar el programa de métricas");
        exit(1);
    }
    else if (monitorID > 0)
    { // Proceso padre, no espera al hijo
        printf("Monitor iniciado con PID %d\n", monitorID);
    }
    else
    {
        perror("Error al crear el proceso para metrics");
    }
}

void stopMonitor()
{
    const char* process_name = "metrics";
    char command[MAX_ARGS];
    snprintf(command, sizeof(command), "killall %s", process_name);

    int result = system(command);

    if (result == -1)
    {
        perror("Error al ejecutar el comando killall");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Proceso '%s' detenido correctamente.\n", process_name);
    }
}
