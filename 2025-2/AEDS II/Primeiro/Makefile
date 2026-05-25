# 
# Makefile para compilar e executar o programa em C
# Comandos principais:
#   make        -> compila o programa e gera o executável
#   make clean  -> remove arquivos objetos e o executável
#   make run    -> compila e executa o programa
#

# Compilador
CC = gcc

# Opções de compilação (avisos e debug)
CFLAGS := -Wall -Wextra -g

# Diretórios
OUTPUT   := output
SRC      := src
INCLUDE  := include

# Ajustes para Windows ou Linux
ifeq ($(OS),Windows_NT)
	MAIN       := main.exe
	SOURCEDIRS := $(SRC)
	INCLUDEDIRS:= $(INCLUDE)
	FIXPATH    = $(subst /,\,$1)
	RM         := del /q /f
	MD         := mkdir
else
	MAIN       := main
	SOURCEDIRS := $(shell find $(SRC) -type d)
	INCLUDEDIRS:= $(shell find $(INCLUDE) -type d)
	FIXPATH    = $1
	RM         = rm -f
	MD         := mkdir -p
endif

# Diretórios de includes (-I)
INCLUDES := $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# Arquivos fonte e objetos
SOURCES  := $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))
OBJECTS  := $(SOURCES:.c=.o)
DEPS     := $(OBJECTS:.o=.d)

# Caminho do executável
OUTPUTMAIN := $(call FIXPATH,$(OUTPUT)/$(MAIN))

# Alvo padrão
all: $(OUTPUT) $(MAIN)
	@echo Compilação concluída!

# Cria diretório de saída
$(OUTPUT):
	$(MD) $(OUTPUT)

# Compila o executável
$(MAIN): $(OBJECTS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS)

# Inclui dependências geradas
-include $(DEPS)

# Regra para compilar cada .c em .o (e gerar dependência .d)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -MMD $< -o $@

# Limpeza
.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	@echo Limpeza concluída!

# Compilar e executar
run: all
	./$(OUTPUTMAIN)
	@echo Execução finalizada!
