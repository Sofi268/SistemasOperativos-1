#include <memory.h>

typedef struct s_block* t_block;
int registro_malloc = 0;
int registro_free = 0;
int registro_realloc = 0;
int registro_calloc = 0;

t_block base = NULL;
t_log_entry* log_head = NULL;
int method = 0;

t_block find_block(t_block* last, size_t size)
{
    t_block b = base;

    switch (method)
    {
    case FIRST_FIT:
        while (b && !(b->free && b->size >= size))
        {
            *last = b;
            b = b->next;
        }
        return b;

    case BEST_FIT: {
        size_t dif = PAGESIZE;
        t_block best = NULL;
        while (b)
        {
            if (b->free)
            {
                if (b->size == size)
                    return b;
                if (b->size > size && (b->size - size) < dif)
                {
                    dif = b->size - size;
                    best = b;
                }
            }
            *last = b;
            b = b->next;
        }
        return best;
    }

    case WORST_FIT: {
        size_t max_size = 0;
        t_block worst = NULL;
        while (b)
        {
            if (b->free && b->size >= size)
            {
                if (b->size > max_size)
                {
                    max_size = b->size;
                    worst = b;
                }
            }
            *last = b;
            b = b->next;
        }
        return worst;
    }

    default:
        fprintf(stderr, "Error: Método de asignación desconocido: %d\n", method);
        return NULL;
    }
}

void split_block(t_block b, size_t s)
{
    if (b->size <= s + BLOCK_SIZE)
    {
        return;
    }

    t_block new = (t_block)(b->data + s);
    new->size = b->size - s - BLOCK_SIZE;
    new->next = b->next;
    new->prev = b;
    new->free = 1;
    new->ptr = new->data;

    if (b->next)
    {
        b->next->prev = new;
    }

    b->size = s;
    b->next = new;
}

void copy_block(t_block src, t_block dst)
{
    size_t i;
    char* sdata = (char*)src->ptr;
    char* ddata = (char*)dst->ptr;
    size_t n = src->size < dst->size ? src->size : dst->size;

    for (i = 0; i < n; i++)
        ddata[i] = sdata[i];
}

t_block get_block(void* p)
{
    t_block b = base;
    while (b)
    {
        if (b->ptr == p)
            return b;
        b = b->next;
    }
    return NULL;
}

int valid_addr(void* p)
{
    if (base)
    {
        if ((char*)p > (char*)base && (char*)p < (char*)sbrk(0))
        {
            t_block b = get_block(p);
            return b && (p == b->ptr);
        }
    }
    return (0);
}

t_block fusion(t_block c)
{
    // Retroceder mientras haya libres antes
    while (c->prev && c->prev->free)
    {
        c = c->prev;
    }

    // Compactar hacia adelante
    while (c->next && c->next->free)
    {
        c->size += BLOCK_SIZE + c->next->size;
        c->next = c->next->next;
        if (c->next)
            c->next->prev = c;
    }
    return c;
}

t_block extend_heap(t_block last, size_t s)
{
    t_block b;
    b = mmap(0, s + BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (b == MAP_FAILED)
    {
        return NULL;
    }
    b->size = s;
    b->next = NULL;
    b->prev = last;
    b->ptr = b->data;

    if (last)
        last->next = b;

    b->free = 0;
    return b;
}

void get_method(int m)
{
    method = m;
}

void set_method(int m)
{
    method = m;
}

void malloc_control(int m)
{
    if (m == 0)
        set_method(FIRST_FIT);
    else if (m == 1)
        set_method(BEST_FIT);
    else if (m == 2)
        set_method(WORST_FIT);
    else
        printf("Error: invalid method\n");
}

void* malloc(size_t size)
{
    registro_malloc++;
    t_block b, last;
    size_t s;
    s = align(size);

    add_log("malloc", s, (unsigned int)registro_malloc);

    if (base)
    {
        last = base;
        b = find_block(&last, s);
        if (b)
        {
            if ((b->size - s) >= (BLOCK_SIZE + 4))
                split_block(b, s);
            b->free = 0;
        }
        else
        {
            b = extend_heap(last, s);
            if (!b)
                return (NULL);
        }
    }
    else
    {
        b = extend_heap(NULL, s);
        if (!b)
            return (NULL);
        base = b;
    }
    return (b->data);
}

void free(void* ptr)
{
    registro_free++;
    add_log("free", 0, (unsigned int)registro_free);
    if (valid_addr(ptr))
    {
        t_block c = get_block(ptr);
        c->free = 1;
        c = fusion(c);
        if (!c->prev)
            base = c;
    }
}

void* calloc(size_t nitems, size_t size)
{
    size_t total = nitems * size;
    registro_calloc++;
    add_log("calloc", total, (unsigned int)registro_calloc);
    // printf("[DEBUG] calloc: nitems=%zu, size=%zu, total=%zu\n", nitems, size, total);

    void* ptr = malloc(total); // reutiliza tu malloc
    if (!ptr)
    {
        // printf("[DEBUG] calloc: malloc falló\n");
        return NULL;
    }

    memset(ptr, 0, total); // inicializa a cero

    // printf("[DEBUG] calloc: ptr=%p inicializado a cero\n", ptr);
    return ptr;
}

void* realloc(void* ptr, size_t size)
{
    registro_realloc++;
    add_log("realloc", size, (unsigned int)registro_realloc);
    // printf("[DEBUG] realloc: ptr=%p, size=%zu\n", ptr, size);

    if (!ptr)
    {
        // printf("[DEBUG] realloc: ptr NULL, llama a malloc\n");
        return malloc(size); // caso NULL -> malloc
    }

    if (size == 0)
    { // tamaño 0 -> free
        free(ptr);
        // printf("[DEBUG] realloc: size 0, ptr liberado\n");
        return NULL;
    }

    t_block b = get_block(ptr); // obtiene la cabecera
    if (!b)
    {
        // printf("[DEBUG] realloc: no se encontró bloque\n");
        return NULL;
    }

    // printf("[DEBUG] realloc: bloque actual size=%zu\n", b->size);

    // Si el bloque actual ya tiene suficiente tamaño
    if (b->size >= size)
    {
        // printf("[DEBUG] realloc: tamaño suficiente, retorno original\n");
        return ptr;
    }

    // Intentar fusionar con el siguiente bloque libre
    if (b->next && b->next->free && (b->size + sizeof(struct s_block) + b->next->size) >= size)
    {
        // printf("[DEBUG] realloc: fusionando con siguiente bloque libre\n");
        b->size += sizeof(struct s_block) + b->next->size;
        b->next = b->next->next;
        if (b->next)
            b->next->prev = b;
        return ptr;
    }

    // Si no alcanza, malloc nuevo y copia
    void* new_ptr = malloc(size);
    if (!new_ptr)
    {
        // printf("[DEBUG] realloc: malloc nuevo falló\n");
        return NULL;
    }

    memcpy(new_ptr, ptr, b->size); // preserva contenido
    free(ptr);

    // printf("[DEBUG] realloc: nuevo ptr=%p, contenido copiado, ptr antiguo liberado\n", new_ptr);
    return new_ptr;
}

void check_heap(void* data)
{
    if (data == NULL)
    {
        printf("Data is NULL\n");
        return;
    }

    t_block block = get_block(data);

    if (block == NULL)
    {
        printf("Block is NULL\n");
        return;
    }

    printf("\033[1;33mHeap check\033[0m\n");
    printf("Size: %zu\n", block->size);

    if (block->next != NULL)
    {
        printf("Next block: %p\n", (void*)(block->next));
    }
    else
    {
        printf("Next block: NULL\n");
    }

    if (block->prev != NULL)
    {
        printf("Prev block: %p\n", (void*)(block->prev));
    }
    else
    {
        printf("Prev block: NULL\n");
    }

    printf("Free: %d\n", block->free);

    if (block->ptr != NULL)
    {
        printf("Beginning data address: %p\n", block->ptr);
        printf("Last data address: %p\n", (void*)((char*)(block->ptr) + block->size));
    }
    else
    {
        printf("Data address: NULL\n");
    }

    printf("Heap address: %p\n", sbrk(0));

    // Extension de funcionalidades
    if (block->prev && block->prev->free && block->free)
    {
        printf("Prev block able for fusion");
    }

    if (block->next && block->next->free && block->free)
    {
        printf("Next block able for fusion");
    }

    if (block->size != align(sizeof(block->data)))
    {
        printf("Real block size does not match with the theoretical one");
    }
}

void memory_usage()
{
    // Se recorre la lista de bloques, se identifica los bloques libres y ocupados y se suman sus tamaños
    t_block b = base;
    size_t freem = 0;
    size_t usedm = 0;

    while (b)
    {
        if (b->free)
        {
            freem += b->size;
        }
        else
        {
            usedm += b->size;
        }
        b = b->next;
    }

    printf("Total used memory: %zu bytes\n", usedm);
    printf("Total free memory: %zu bytes\n", freem);
}

void add_log(const char* op, size_t size, unsigned int counter)
{
    t_log_entry* new_log = mmap(NULL, sizeof(t_log_entry), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_log == MAP_FAILED)
        return;

    strncpy(new_log->op, op, sizeof(new_log->op) - 1);
    new_log->op[sizeof(new_log->op) - 1] = '\0';
    new_log->size = size;
    new_log->timestamp = time(NULL);
    new_log->counter = counter;

    new_log->next = log_head;
    log_head = new_log;
}

void print_logs()
{
    t_log_entry* curr = log_head;
    while (curr)
    {
        printf("[%ld] %s (%zu bytes) #%u\n", curr->timestamp, curr->op, curr->size, curr->counter);
        curr = curr->next;
    }
}

void free_logs()
{
    t_log_entry* curr = log_head;
    t_log_entry* tmp;
    while (curr)
    {
        tmp = curr;
        curr = curr->next;
        munmap(tmp, sizeof(t_log_entry));
    }
    log_head = NULL;
}

void memory_usage_stats()
{
    printf("Memory usage statistics:\n");
    printf("Malloc calls  : %d\n", registro_malloc);
    printf("Free calls    : %d\n", registro_free);
    printf("Calloc calls  : %d\n", registro_calloc);
    printf("Realloc calls : %d\n", registro_realloc);
    memory_usage();
}

