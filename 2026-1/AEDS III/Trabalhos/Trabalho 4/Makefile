CC = gcc

CFLAGS = -Wall -Wextra -O2 -Iinclude
DEBUGFLAGS = -g -Wall -Wextra -Iinclude

# ==========================================================
# ARQUIVOS
# ==========================================================

# Todos os .c EXCETO instancias.c
SRC = $(filter-out src/instancias.c, $(wildcard src/*.c))

TARGET = bin/main
GENERATOR = bin/instancias

# ==========================================================
# PROGRAMA PRINCIPAL
# ==========================================================

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

debug:
	@mkdir -p bin
	$(CC) $(DEBUGFLAGS) $(SRC) -o $(TARGET)

valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# ==========================================================
# GERADOR DE INSTÂNCIAS
# ==========================================================

instancias: $(GENERATOR)
	./$(GENERATOR)

$(GENERATOR): src/instancias.c
	@mkdir -p bin
	$(CC) $(CFLAGS) src/instancias.c -o $(GENERATOR)

# ==========================================================
# LIMPEZA
# ==========================================================

clean:
	@echo "Removendo executáveis..."
	rm -rf bin

	@echo "Removendo instâncias geradas..."
	rm -rf instances

	@echo "Removendo resultados..."
	rm -rf Results

	@echo "Limpeza completa!"

rebuild: clean all