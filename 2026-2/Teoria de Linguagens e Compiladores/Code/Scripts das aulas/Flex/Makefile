# Makefile — Exemplos de Flex
#
# Compila cada um dos exemplos .l desta pasta e oferece alvos para
# rodar todos de uma vez (com teste.txt) ou limpar os arquivos gerados.

CC     = gcc
LEX    = flex
LDLIBS = -lfl
TESTE  = teste.txt

# Nomes-base dos exemplos (sem a extensao .l).
EXEMPLOS = 01_regra_padrao \
           02_extrai_numeros \
           03_numera_linhas \
           04_conta_identificadores \
           05_conta_texto

# Arquivos .c gerados pelo flex e binarios finais, a partir da lista acima.
FONTES_C  = $(addsuffix .c,$(EXEMPLOS))
BINARIOS  = $(EXEMPLOS)

.PHONY: all build run clean

all: build

# Compila todos os exemplos
build: $(BINARIOS)

# Regra generica: qualquer NOME.c vem de NOME.l via flex
%.c: %.l
	$(LEX) -o $@ $<

# Regra generica: qualquer binario NOME vem de NOME.c via gcc
%: %.c
	$(CC) $< -o $@ $(LDLIBS)

# Roda todos os exemplos, um apos o outro, usando teste.txt como entrada.
# 03_numera_linhas e diferente dos demais: ele le o nome do arquivo pela
# linha de comando (argv[1]), em vez de ler pela entrada padrao (stdin).
run: build
	@echo
	@echo "===== 01_regra_padrao ====="
	@printf '%s\n' "`./01_regra_padrao < $(TESTE)`"
	@echo
	@echo "===== 02_extrai_numeros ====="
	@printf '%s\n' "`./02_extrai_numeros < $(TESTE)`"
	@echo
	@echo "===== 03_numera_linhas ====="
	@printf '%s\n' "`./03_numera_linhas $(TESTE)`"
	@echo
	@echo "===== 04_conta_identificadores ====="
	@printf '%s\n' "`./04_conta_identificadores < $(TESTE)`"
	@echo
	@echo "===== 05_conta_texto ====="
	@printf '%s\n' "`./05_conta_texto < $(TESTE)`"
	@echo

# Remove os .c gerados e os binarios, mantendo os .l e o teste.txt
clean:
	rm -f $(FONTES_C) $(BINARIOS)