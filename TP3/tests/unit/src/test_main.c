#include "memory.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_ALLOCS 200
#define MAX_SIZE 256

// Se corre antes de cada test
void setUp(void)
{
    registro_malloc = 0;
    registro_free = 0;
    registro_calloc = 0;
    registro_realloc = 0;
    free_logs();
}

// Se corre después de cada test
void tearDown(void)
{
    free_logs();
}

// Test 1: malloc simple
void test_malloc_and_write(void)
{
    void* a = malloc(64);
    TEST_ASSERT_NOT_NULL(a);

    memset(a, 0xAB, 64);
    unsigned char* pa = (unsigned char*)a;
    for (int i = 0; i < 64; i++)
    {
        TEST_ASSERT_EQUAL_HEX8(0xAB, pa[i]);
    }

    free(a);
    TEST_ASSERT_EQUAL(1, registro_malloc);
    TEST_ASSERT_EQUAL(1, registro_free);
}

// Test 2: calloc inicializa a cero
void test_calloc(void)
{
    int* arr = calloc(10, sizeof(int));
    TEST_ASSERT_NOT_NULL(arr);

    for (int i = 0; i < 10; i++)
    {
        TEST_ASSERT_EQUAL(0, arr[i]);
    }

    free(arr);
    TEST_ASSERT_EQUAL(1, registro_malloc);
    TEST_ASSERT_EQUAL(1, registro_free);
}

// Test 3: realloc expandiendo
void test_realloc_expand(void)
{
    int* arr = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
        arr[i] = i;

    arr = realloc(arr, 20 * sizeof(int));
    TEST_ASSERT_NOT_NULL(arr);

    for (int i = 0; i < 10; i++)
    {
        TEST_ASSERT_EQUAL(i, arr[i]);
    }

    free(arr);

    TEST_ASSERT_EQUAL(2, registro_malloc);  // malloc inicial + malloc interno
    TEST_ASSERT_EQUAL(1, registro_realloc); // una llamada a realloc
    TEST_ASSERT_EQUAL(2, registro_free);    // free interno + free final
}

// Test 4: fusión de bloques libres
void test_merge_blocks(void)
{
    void* c1 = malloc(128);
    void* c2 = malloc(128);
    TEST_ASSERT_NOT_NULL(c1);
    TEST_ASSERT_NOT_NULL(c2);

    free(c1);
    free(c2);

    void* c3 = malloc(200);
    TEST_ASSERT_NOT_NULL(c3);

    free(c3);
}

// Test 5: logs se registran y liberan
void test_logs(void)
{
    void* p = malloc(32);
    free(p);

    TEST_ASSERT_NOT_NULL(log_head);
    TEST_ASSERT_EQUAL_STRING("free", log_head->op);

    free_logs();
    TEST_ASSERT_NULL(log_head);
}

// Test 6: memory_usage_stats imprime sin fallar
void test_memory_usage_stats(void)
{
    void* p1 = malloc(50);
    void* p2 = calloc(5, 10);
    void* p3 = realloc(p1, 100);
    free(p2);
    free(p3);

    memory_usage_stats();
    TEST_ASSERT_GREATER_THAN(0, registro_malloc);
    TEST_ASSERT_GREATER_THAN(0, registro_calloc);
    TEST_ASSERT_GREATER_THAN(0, registro_realloc);
    TEST_ASSERT_GREATER_THAN(0, registro_free);
}

// Test 7: check_heap
void test_check_heap(void)
{
    void* p = malloc(64);
    check_heap(p);
    free(p);
}

// Helpers para medir tiempo
static inline long long get_time_ns()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

void run_policy_test(int policy, const char* name)
{
    malloc_control(policy);

    void* ptrs[NUM_ALLOCS];
    long long start = get_time_ns();

    for (int i = 0; i < NUM_ALLOCS; i++)
    {
        size_t size = (size_t)(rand() % MAX_SIZE) + 1;
        ptrs[i] = malloc(size);
        if (ptrs[i])
        {
            memset(ptrs[i], 0xAB, size);
        }
        if (i % 3 == 0 && ptrs[i])
        {
            free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }

    long long end = get_time_ns();
    double ms = (double)(end - start) / 1e6;

    // medir fragmentación
    size_t free_mem = 0, used_mem = 0, max_free_block = 0;
    t_block b = base;
    while (b)
    {
        if (b->free)
        {
            free_mem += b->size;
            if (b->size > max_free_block)
                max_free_block = b->size;
        }
        else
        {
            used_mem += b->size;
        }
        b = b->next;
    }

    double frag_ext = 0.0;
    if (max_free_block > 0 && free_mem > 0)
    {
        frag_ext = (double)(free_mem - max_free_block) / (double)free_mem;
    }

    printf("=== %s ===\n", name);
    printf("Tiempo total: %.2f ms\n", ms);
    printf("Memoria usada: %zu bytes, libre: %zu bytes\n", used_mem, free_mem);
    printf("Fragmentación externa: %.2f%%\n\n", frag_ext * 100.0);
}

// Test 8: policies comparison
void test_policy_efficiency(void)
{
    srand((unsigned int)time(NULL));
    run_policy_test(FIRST_FIT, "First Fit");
    run_policy_test(BEST_FIT, "Best Fit");
    run_policy_test(WORST_FIT, "Worst Fit");
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_malloc_and_write);
    RUN_TEST(test_calloc);
    RUN_TEST(test_realloc_expand);
    RUN_TEST(test_merge_blocks);
    RUN_TEST(test_logs);
    RUN_TEST(test_memory_usage_stats);
    RUN_TEST(test_check_heap);
    RUN_TEST(test_policy_efficiency);

    return UNITY_END();
}


