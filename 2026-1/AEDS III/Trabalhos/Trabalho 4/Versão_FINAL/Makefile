# =======================================================================================
#               INSTRUÇÕES DE USO DO MAKEFILE (MAPEAMENTO DE COMANDOS)
# =======================================================================================
# 'make'            -> Compila o programa principal e gera o executável dentro de 'output'
# 'make instancias' -> Compila e roda o gerador automático que cria os arquivos de teste
# 'make clean'      -> Remove todos os arquivos temporários (.o, .d), binários e pastas geradas
# =======================================================================================

# Define o compilador de C que será utilizado no projeto
CC = gcc

# Define as flags de compilação:
# -Wall: Ativa os avisos (warnings) estruturais comuns
# -Wextra: Ativa avisos adicionais mais rígidos de boas práticas
# -g: Inclui informações de depuração (permite usar o GDB/Valgrind para debugar)
CFLAGS  := -Wall -Wextra -g

# Define caminhos e flags de bibliotecas externas (-lm vincula a biblioteca matemática <math.h>)
LFLAGS = -lm

# Definição da árvore de diretórios padrão do projeto
OUTPUT  := output
SRC     := src
INCLUDE := include
LIB     := lib

# =======================================================================================
#          BLOCO MULTIPLATAFORMA: DETECÇÃO AUTOMÁTICA DO SISTEMA OPERACIONAL
# =======================================================================================
ifeq ($(OS),Windows_NT)
    # --- Configurações nativas para ambiente Windows ---
    MAIN        := main.exe
    SOURCEDIRS  := $(SRC)
    INCLUDEDIRS := $(INCLUDE)
    LIBDIRS     := $(LIB)
    # Função interna para substituir barras normais (/) por barras invertidas (\) exigidas pelo CMD
    FIXPATH     = $(subst /,\,$1)
    RM          := del /q /f
    MD          := mkdir
    
    # Variáveis dedicadas ao utilitário gerador de instâncias
    GERADOR     := tools\instancias.exe
    RUN_GERADOR := cd tools && instancias.exe
    RMDIR       := rmdir /s /q
else
    # --- Configurações nativas para ambiente POSIX (Linux / macOS) ---
    MAIN        := main
    # Usa o comando shell 'find' para mapear recursivamente todos os subdiretórios
    SOURCEDIRS  := $(shell find $(SRC) -type d)
    INCLUDEDIRS := $(shell find $(INCLUDE) -type d)
    LIBDIRS     := $(shell find $(LIB) -type d)
    # Nos sistemas Unix, as barras normais (/) já estão corretas, então a função mantém o texto intacto
    FIXPATH     = $1
    RM          = rm -f
    MD          := mkdir -p # O parâmetro '-p' cria pastas pais caso não existam e evita erros se já existirem
    
    # Variáveis dedicadas ao utilitário gerador de instâncias
    GERADOR     := tools/instancias
    RUN_GERADOR := cd tools && ./instancias
    RMDIR       := rm -rf
endif
# =======================================================================================

# Adiciona o prefixo '-I' antes de cada diretório de include para avisar o GCC onde buscar os headers (.h)
INCLUDES    := $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# Adiciona o prefixo '-L' antes de cada diretório para mapear os caminhos de bibliotecas estáticas (.a / .lib)
LIBS        := $(patsubst %,-L%, $(LIBDIRS:%/=%))

# Mapeia dinamicamente todos os arquivos de código-fonte (.c) encontrados nas pastas de origem
SOURCES     := $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))

# Define que para cada arquivo .c listado, haverá um arquivo correspondente de código objeto (.o)
OBJECTS     := $(SOURCES:.c=.o)

# Define arquivos de dependência (.d). Eles guardam quais cabeçalhos cada arquivo .c utiliza
DEPS        := $(OBJECTS:.o=.d)

# Normaliza o caminho final do executável principal aplicando as barras corretas do SO
OUTPUTMAIN  := $(call FIXPATH,$(OUTPUT)/$(MAIN))


# =======================================================================================
#                               REGRAS E ALVOS DE COMPILAÇÃO
# =======================================================================================

# Regra padrão executada ao digitar apenas 'make' ou 'make all'
all: $(OUTPUT) $(MAIN)
	@echo Execução completa realizada!

# Cria o diretório de saída binária caso ele não exista
$(OUTPUT):
	$(MD) $(OUTPUT)

# Linkagem Final: Junta todos os arquivos objeto (.o) coletados e constrói o executável principal
$(MAIN): $(OBJECTS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

# Compila o utilitário isolado que gera os arquivos de instâncias de teste
$(GERADOR): tools/instancias.c
	$(CC) $(CFLAGS) tools/instancias.c -o $(GERADOR)

# Comando 'make instancias': Compila o gerador e o executa para popular a pasta de testes
.PHONY: instancias
instancias: $(GERADOR)
	$(RUN_GERADOR)
	@echo Instancias geradas com sucesso!

# Inclui os arquivos de dependência (.d). Se um arquivo .h mudar, o Make sabe que precisa re-compilar o .c dele
-include $(DEPS)

# Regra Implícita: Compila de forma incremental cada arquivo .c em seu respectivo .o
# A flag -MMD diz ao GCC para criar as árvores de dependência dinamicamente enquanto compila
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -MMD $<  -o $@

# Comando 'make clean': Faxina completa do projeto para resetar o espaço de trabalho
.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	$(RM) $(call FIXPATH,$(GERADOR))
	-$(RMDIR) $(call FIXPATH,instancias) # O hífen '-' antes do comando ignora erros caso a pasta não exista
	-$(RMDIR) $(call FIXPATH,resultados)  # Remove os relatórios gerados pelas baterias de testes
	@echo Limpeza completa!

# Comando experimental 'make run': Compila tudo e já executa o programa imediatamente
run: all
	./$(OUTPUTMAIN)
	@echo Execucao 'run: all' completa!