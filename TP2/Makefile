# Nombre del compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -Wextra -g -Iinclude

# Directorios
SRC_DIR = src
INC_DIR = include

# Archivos fuente
SRC = $(wildcard $(SRC_DIR)/*.c)

# Archivos objeto (automáticamente generados)
OBJ = $(SRC:.c=.o)

# Nombre del ejecutable
EXEC = my_shell

# Regla para compilar el ejecutable
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Regla para compilar archivos objeto
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/shell.h
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para limpiar archivos generados
clean:
	rm -f $(OBJ) $(EXEC)

# Regla para ejecutar el programa
run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run
