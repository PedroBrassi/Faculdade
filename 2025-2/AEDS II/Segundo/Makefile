# ====================================================
# UNIVERSIDADE FEDERAL DE ALFENAS – UNIFAL-MG
# Bacharelado em Ciência da Computação
# Disciplina: Algoritmos e Estruturas de Dados II
# ----------------------------------------------------
# Makefile - Trabalho Prático 2: Ordenação Externa
# ----------------------------------------------------
# Este Makefile compila o projeto localizado em "src/"
# e gera o executável na pasta "bin/".
# ====================================================

# Compilador e flags
CC = gcc
CFLAGS = -Wall -O2

# Caminhos
SRC_DIR = src
BIN_DIR = bin
TARGET = $(BIN_DIR)/ordenacao_externa

# Arquivos fonte e cabeçalhos
SRC = $(SRC_DIR)/main.c
HDR = $(SRC_DIR)/ordenacaoExterna.h

# Regra padrão
all: $(BIN_DIR) $(TARGET)

# Compilação do executável
$(TARGET): $(SRC) $(HDR)
	@echo "Compilando o projeto..."
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
	@echo "Compilação concluída com sucesso!"
	@echo "Executável gerado em: $(TARGET)"

# Cria o diretório bin caso não exista
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Limpa os arquivos gerados
clean:
	@echo "Removendo arquivos binários e temporários..."
	rm -rf $(BIN_DIR)
	rm -f *.bin *.txt
	@echo "Limpeza concluída!"

# Executa o programa após a compilação
run: all
	@echo "Executando o programa..."
	./$(TARGET)

# ================
# Fim do Makefile
# ================
