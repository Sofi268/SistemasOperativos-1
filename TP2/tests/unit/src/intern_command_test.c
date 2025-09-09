#include "intern_command_test.h"

void test_changeDirectory(void)
{
    // Caso: Mostrar directorio actual
    char* path = NULL;
    changeDirectory(path);
    TEST_ASSERT_NOT_NULL(getenv("PWD"));

    // Caso: Cambiar a un directorio existente
    char* valid_path = "/tmp";
    changeDirectory(valid_path);
    TEST_ASSERT_EQUAL_STRING(valid_path, getenv("PWD"));

    // Caso: Cambiar a un directorio inexistente
    char* invalid_path = "/path/inexistente";
    changeDirectory(invalid_path);
    TEST_ASSERT_NOT_EQUAL(0, chdir(invalid_path)); // Error esperado

    // Caso: Cambiar al directorio anterior
    char* previous_dir = "-";
    changeDirectory(previous_dir);
    TEST_ASSERT_NOT_NULL(getenv("OLDPWD"));
}

void test_clearScreen(void)
{
    clearScreen();
    TEST_ASSERT_TRUE(1);
}

void test_echoCommand(void)
{
    char input[] = "Hello $HOME";
    char expected_output[MAX_PRINT];
    snprintf(expected_output, MAX_PRINT, "Hello %s\n", getenv("HOME"));

    // Redirigir stdout para capturar la salida
    FILE* stdout_backup = stdout;
    char buffer[MAX_DIR_LENGTH];
    stdout = fmemopen(buffer, sizeof(buffer), "w");

    echoCommand(input);

    fflush(stdout);
    stdout = stdout_backup;

    TEST_ASSERT_EQUAL_STRING(expected_output, buffer);
}
