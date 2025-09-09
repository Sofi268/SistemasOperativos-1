#include "shell_test.h"

void test_strip_quotes(void)
{
    char test1[] = "\"test\"";
    char test2[] = "'example'";
    char test3[] = "no_quotes";

    strip_quotes(test1);
    strip_quotes(test2);
    strip_quotes(test3);

    TEST_ASSERT_EQUAL_STRING("test", test1);
    TEST_ASSERT_EQUAL_STRING("example", test2);
    TEST_ASSERT_EQUAL_STRING("no_quotes", test3);
}

void get_Prompt_Test(void)
{
    char cwd[MAX_LENGTH];
    assert(getcwd(cwd, sizeof(cwd)) != NULL); // Verifica que getcwd no falle

    char* prompt = getPrompt();
    assert(prompt != NULL); // Verifica que el prompt no sea NULL

    char* dir_in_prompt = strstr(prompt, cwd);
    assert(dir_in_prompt != NULL); // Verifica que el directorio actual esté en el prompt

    printf("El test get_Prompt_Test pasó correctamente.\n");
}

void test_changeSettings(void)
{
    const char* test_filename = "test_config.json";

    // Crear archivo de configuración de prueba
    FILE* file = fopen(test_filename, "w");
    TEST_ASSERT_NOT_NULL(file);
    fprintf(file, "{\n"
                  "  \"sampling_interval\": 10,\n"
                  "  \"exposed_metrics\": [\"cpu\", \"mem\"],\n"
                  "  \"MemTotal\": 8000\n"
                  "}");
    fclose(file);

    // Nuevos valores
    int new_interval = 20;
    const char* new_metrics[] = {"disk", "network"};
    int metrics_count = 2;
    int new_memtotal = 16000;

    // Llamar a la función
    changeSettings(new_interval, new_metrics, metrics_count, new_memtotal, test_filename);

    // Leer el archivo modificado para validar los cambios
    file = fopen(test_filename, "r");
    TEST_ASSERT_NOT_NULL(file);

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* updated_content = malloc(file_size + 1);
    TEST_ASSERT_NOT_NULL(updated_content);
    size_t bytesRead = fread(updated_content, 1, file_size, file);
    if (bytesRead != file_size)
    {
        fprintf(stderr, "Error: No se pudo leer correctamente el archivo. Se leyeron %zu de %zu bytes.\n", bytesRead,
                file_size);
    }
    updated_content[file_size] = '\0';
    fclose(file);

    // Parsear el JSON actualizado
    cJSON* updated_config = cJSON_Parse(updated_content);
    free(updated_content);
    TEST_ASSERT_NOT_NULL(updated_config);

    // Validar cambios
    cJSON* sampling_interval = cJSON_GetObjectItem(updated_config, "sampling_interval");
    TEST_ASSERT_NOT_NULL(sampling_interval);
    TEST_ASSERT_EQUAL_INT(new_interval, sampling_interval->valueint);

    cJSON* exposed_metrics = cJSON_GetObjectItem(updated_config, "exposed_metrics");
    TEST_ASSERT_NOT_NULL(exposed_metrics);
    TEST_ASSERT_EQUAL_INT(metrics_count, cJSON_GetArraySize(exposed_metrics));
    TEST_ASSERT_EQUAL_STRING("disk", cJSON_GetArrayItem(exposed_metrics, 0)->valuestring);
    TEST_ASSERT_EQUAL_STRING("network", cJSON_GetArrayItem(exposed_metrics, 1)->valuestring);

    cJSON* memtotal = cJSON_GetObjectItem(updated_config, "MemTotal");
    TEST_ASSERT_NOT_NULL(memtotal);
    TEST_ASSERT_EQUAL_INT(new_memtotal, memtotal->valueint);

    cJSON_Delete(updated_config);

    remove(test_filename);
}
