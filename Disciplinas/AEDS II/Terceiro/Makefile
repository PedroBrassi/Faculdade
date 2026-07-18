#
# Makefile para compilar projeto em src/ com saída em output/
#

# Compilador
CC = gcc

# Flags de compilação
CFLAGS := -Wall -Wextra -g -Iinclude -Wno-unused-parameter

# Flags de link (em branco por enquanto)
LFLAGS =

# Diretórios
OUTPUT := output
SRC := src
INCLUDE := include
LIB := lib

# Compatibilidade Windows/Linux
ifeq ($(OS),Windows_NT)
	MAIN := main.exe
	RM := del /q /f
	MD := mkdir
	FIXPATH = $(subst /,\,$1)
else
	MAIN := main
	RM := rm -f
	MD := mkdir -p
	FIXPATH = $1
endif

# Descobrir subdiretórios automaticamente
SOURCEDIRS := $(shell find $(SRC) -type d)
INCLUDEDIRS := $(shell find $(INCLUDE) -type d)
LIBDIRS := $(shell find $(LIB) -type d)

# -I e -L
INCLUDES := $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
LIBS := $(patsubst %,-L%, $(LIBDIRS:%/=%))

# Fontes e objetos
SOURCES := $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))
OBJECTS := $(SOURCES:.c=.o)
DEPS := $(OBJECTS:.o=.d)

# Caminho final do executável
OUTPUTMAIN := $(call FIXPATH,$(OUTPUT)/$(MAIN))

# Alvo padrão
all: $(OUTPUT) $(OUTPUTMAIN)
	@echo "Compilação concluída com sucesso!"

# Criar pasta output se não existir
$(OUTPUT):
	$(MD) $(OUTPUT)

# Linkagem final
$(OUTPUTMAIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

# Gerar .o + .d a partir de .c
# -MMD gera dependências automaticamente
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -MMD $< -o $@

# Incluir dependências
-include $(DEPS)

# Limpeza
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(OBJECTS)
	$(RM) $(DEPS)
	@echo "Cleanup complete!"

# Executar
run: all
	./$(OUTPUTMAIN)
	@echo "Programa executado."
