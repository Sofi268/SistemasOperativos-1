#include "intern_command.h"

void changeDirectory(char* path)
{
    char* oldpwd = getenv("PWD");
    char* prevpwd = getenv("OLDPWD");

    if (path == NULL || strcmp(path, "") == 0)
    {
        // Sin directorio, se muestra el directorio actual
        printf("Directorio actual: %s\n", oldpwd);
    }
    else if (strcmp(path, "-") == 0)
    {
        // Cambiar al directorio anterior
        if (prevpwd != NULL)
        {
            if (chdir(prevpwd) == 0)
            {
                setenv("OLDPWD", oldpwd, 1);
                setenv("PWD", prevpwd, 1);
            }
            else
            {
                perror("Error al cambiar al último directorio de trabajo");
            }
        }
        else
        {
            fprintf(stderr, "No hay un directorio anterior definido.\n");
        }
    }
    else if (chdir(path) == 0)
    {
        // Cambiar directorio
        setenv("OLDPWD", oldpwd, 1);
        char newpwd[MAX_DIR_LENGTH];
        if (getcwd(newpwd, sizeof(newpwd)) != NULL)
        {
            setenv("PWD", newpwd, 1);
        }
        else
        {
            perror("Error al obtener el directorio actual");
        }
    }
    else
    {
        perror("Error al cambiar de directorio");
    }
}

void clearScreen()
{
    if (system("clear") == -1)
    {
        perror("Error al limpiar la pantalla");
    }
}

void echoCommand(char* arg)
{
    if (arg == NULL)
    {
        fprintf(stderr, "Error: argumento vacío\n");
        return;
    }

    char result[MAX_PRINT] = "";    // Buffer para almacenar el resultado final
    char* token = strtok(arg, " "); // Dividir la entrada en tokens separadas por espacios

    while (token != NULL)
    {
        if (token[0] == '$')
        {
            // Es una variable de entorno
            char* env_value = getenv(token + 1); // Obtener el valor de la variable sin el '$'
            if (env_value)
            {
                strcat(result, env_value); // Concatenar el valor de la variable al resultado
            }
            else
            {
                strcat(result, ""); // Si no se encuentra la variable, no hacer nada
            }
        }
        else
        {
            // Texto literal
            strcat(result, token); // Concatenar el texto literal al resultado
        }

        strcat(result, " ");       // Agregar un espacio después de cada argumento
        token = strtok(NULL, " "); // Avanzar al siguiente token
    }

    // Imprimir el resultado final, eliminando el espacio extra al final
    if (strlen(result) > 0)
    {
        result[strlen(result) - 1] = '\0'; // Quitar el espacio extra al final
    }

    printf("%s\n", result); // Imprimir el resultado
}

void buscarConfig(char* path, char* operacion){
    DIR* dir = opendir(path);
    if (!dir) {
        perror("No se puede abrir el directorio");
        return;
    }

    struct dirent* entry;
    char fullpath[MAX_DIR_LENGTH];
    int found = 0; // contador de archivos encontrados

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullpath, MAX_DIR_LENGTH, "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) == -1)
            continue;

        if (S_ISREG(st.st_mode)) {
            if (strstr(entry->d_name, ".config") || strstr(entry->d_name, ".json") ||
                strstr(entry->d_name, ".conf")   || strstr(entry->d_name, ".cfg")  ||
                strstr(entry->d_name, ".ini")    || strstr(entry->d_name, ".yaml") ||
                strstr(entry->d_name, ".yml")    || strstr(entry->d_name, ".env")  ||
                strstr(entry->d_name, ".toml")   || strstr(entry->d_name, ".properties")) {

                char abs_path[MAX_DIR_LENGTH];
                if (realpath(fullpath, abs_path)) { // Ruta absoluta
                    found++;
                    if (strcmp(operacion, "paths") == 0) {
                        printf("%s\n", abs_path);
                    } else if (strcmp(operacion, "contenido") == 0) {
                        FILE* f = fopen(abs_path, "r");
                        if (!f) {
                            perror(abs_path);
                            continue;
                        }
                        printf("=== Contenido de %s ===\n", abs_path);
                        char buffer[1024];
                        while (fgets(buffer, sizeof(buffer), f)) {
                            printf("%s", buffer);
                        }
                        printf("\n------------------------\n\n");
                        fclose(f);
                    } else {
                        printf("Operación desconocida: %s\n", operacion);
                    }
                } else {
                    perror("realpath");
                }
            }
        }
    }

    if (found == 0) {
        printf("No se encontraron archivos de configuración en: %s\n", path);
    }

    closedir(dir);
}

void buscarFormato(char* path, const char* formato) {
    if (access(path, R_OK) != 0) {
        return; // Ignora directorios sin permisos
    }
    DIR* dir = opendir(path);
    if (!dir) {
        perror("No se puede abrir el directorio");
        return;
    }

    struct dirent* entry;
    char fullpath[MAX_DIR_LENGTH];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullpath, MAX_DIR_LENGTH, "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) == -1)
            continue;

        if (S_ISDIR(st.st_mode)) {
            // Recursión en subdirectorio
            buscarFormato(fullpath, formato);
        } else if (S_ISREG(st.st_mode)) {
            // Verificar coincidencia con el formato
            if (strstr(entry->d_name, formato)) {
                char abs_path[MAX_DIR_LENGTH];
                if (realpath(fullpath, abs_path)) {
                    printf("%s\n", abs_path);
                } else {
                    perror("realpath");
                }
            }
        }
    }

    closedir(dir);
}

bool isRunning = false;
pid_t idRunning = -1;

bool get_isRunning()
{
    return isRunning;
}

pid_t get_idRunning()
{
    return idRunning;
}

void run_metrics()
{
    // Ejecutar "make" en el submódulo
    pid_t makeID = fork();

    if (makeID == 0)
    { // Proceso hijo
        char* args[] = {"make", "-C", "../submodulos/Tp1-SO1", NULL};
        execvp(args[0], args);
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

    // Ejecutar el programa de métricas
    pid_t monitorID = fork();

    if (monitorID == 0)
    { // Proceso hijo
        char* args[] = {"../submodulos/Tp1-SO1/metrics", NULL};
        execvp(args[0], args);
        perror("Error al ejecutar el programa");
        exit(1);
    }
    else if (monitorID > 0)
    { // Proceso padre
        idRunning = monitorID;
        isRunning = true;
        printf("Monitor iniciado con PID %d\n", idRunning);
    }
    else
    {
        perror("Error al crear el proceso");
    }
}

// Función para iniciar el monitor
void start_monitor()
{
    if (isRunning)
    {
        printf("El monitor ya se estaba ejecutando (PID: %d)\n", idRunning);
    }
    else
    {
        run_metrics();
    }
}

// Función para detener el monitor
void stop_monitor()
{
    if (isRunning)
    {
        // Detener el proceso del monitor
        if (kill(idRunning, SIGKILL) == -1)
        {
            perror("Error al detener el monitor");
        }
        else
        {
            printf("Monitor detenido (PID: %d)\n", idRunning);
            isRunning = false;
            idRunning = -1;
        }
    }
    else
    {
        printf("El monitor ya estaba inactivo\n");
    }

    // Ejecutar "make clean" en el submódulo
    pid_t makeCleanID = fork();

    if (makeCleanID == 0)
    { // Proceso hijo
        char* args[] = {"make", "-C", "../submodulos/Tp1-SO1", "clean", NULL};
        execvp(args[0], args);
        perror("Error al ejecutar make clean");
        exit(1);
    }
    else if (makeCleanID > 0)
    { // Proceso padre
        int status;
        waitpid(makeCleanID, &status, 0); // Esperar a que termine el "make clean"
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            printf("Limpieza completada correctamente\n");
        }
        else
        {
            printf("Error durante la limpieza\n");
        }
    }
    else
    {
        perror("Error al crear el proceso para make clean");
    }
}

// Función para verificar el estado del monitor
void status_monitor()
{
    if (isRunning)
    {
        printf("El monitor está activo con PID %d\n", idRunning);
    }
    else
    {
        printf("El monitor está inactivo\n");
    }
}

// Función principal de la shell que interactúa con el monitor
void shell_interaction()
{
    start_monitor();  // Iniciar el monitor
    sleep(20);        // Simular tiempo de operación
    status_monitor(); // Verificar el estado del monitor
    sleep(1);
    stop_monitor(); // Detener el monitor
    sleep(1);
    status_monitor(); // Verificar el estado del monitor
}
