# Makefile — Analisador Léxico "Simples"

CC      = gcc
CFLAGS  =
LDLIBS  = -lfl
LEX     = flex

SRC_L   = lexico.l
SRC_C   = lexico.c
BIN     = lexico
TESTE   = teste.simples
SAIDA   = result.txt

.PHONY: all build run clean

all: build

# Gera lexico.c a partir de lexico.l e compila o executavel
build: $(BIN)

$(SRC_C): $(SRC_L)
	$(LEX) -o $(SRC_C) $(SRC_L)

$(BIN): $(SRC_C)
	$(CC) $(CFLAGS) $(SRC_C) -o $(BIN) $(LDLIBS)

# Compila (se preciso) e roda o analisador sobre o arquivo de teste
run: build
	./$(BIN) < $(TESTE) > $(SAIDA)
	@echo "Saida gravada em $(SAIDA)"

# Remove arquivos gerados, mantendo apenas o .l e o .simples originais
clean:
	rm -f $(SRC_C) $(BIN) $(SAIDA)