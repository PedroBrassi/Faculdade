# Makefile - TCC: Classificacao de Tumores Cerebrais em Imagens de Ressonancia Magnetica
#
# Automatiza os comandos ja documentados no README.md e nos relatorios da
# pasta Relatorios/. Este Makefile nao altera nenhum script nem dado do
# projeto -- ele apenas chama o que ja existe.
#
# Requer o utilitario `make` disponivel no terminal (Git Bash, WSL ou MSYS2
# no Windows; make ja vem pronto no Linux/macOS).
#
# Uso (a partir da raiz do projeto):
#   make                   -> mostra esta ajuda (alvo padrao, nao executa nada)
#   make venv              -> cria o ambiente virtual em Desenvolvimento/.venv
#   make install           -> instala as dependencias de Desenvolvimento/requirements.txt
#   make poc               -> roda a Prova de Conceito (Etapa 0, 100 imagens)
#   make auditoria         -> roda a auditoria da base completa (Etapa 1)
#   make dedup             -> roda a deduplicacao e analise de sensibilidade (Etapa 2)
#   make all               -> roda auditoria e dedup, nessa ordem
#   make clean-resultados CONFIRM=yes -> apaga os CSVs gerados em resultados/ (Etapas 1 e 2)

DEV_DIR := Desenvolvimento
VENV_DIR := $(DEV_DIR)/.venv
REQUIREMENTS := $(DEV_DIR)/requirements.txt

POC_SCRIPT := $(DEV_DIR)/Etapa 0 - Ambiente e Poc/_PoC/poc_auditoria.py
ETAPA1_SCRIPT := $(DEV_DIR)/Etapa 1 - Auditoria da Base/auditoria.py
ETAPA2_SCRIPT := $(DEV_DIR)/Etapa 2 - Deduplicacao e Sensibilidade/deduplicacao_exata.py

ETAPA1_RESULTADOS := $(DEV_DIR)/Etapa 1 - Auditoria da Base/resultados
ETAPA2_RESULTADOS := $(DEV_DIR)/Etapa 2 - Deduplicacao e Sensibilidade/resultados

# Python do ambiente virtual: caminho do Windows (.venv/Scripts) ou do
# Unix/WSL (.venv/bin), conforme o que existir no disco. Se nenhum existir
# ainda (venv nao criada), cai no caminho do Windows como padrao.
WIN_PYTHON := $(VENV_DIR)/Scripts/python.exe
UNIX_PYTHON := $(VENV_DIR)/bin/python
PYTHON := $(or $(wildcard $(WIN_PYTHON)),$(wildcard $(UNIX_PYTHON)),$(WIN_PYTHON))

.DEFAULT_GOAL := help
.PHONY: help venv install poc auditoria etapa1 dedup deduplicacao etapa2 all clean-resultados

help:
	@echo "Alvos disponiveis:"
	@echo "  make venv                          - cria o ambiente virtual em $(VENV_DIR)"
	@echo "  make install                       - instala as dependencias de $(REQUIREMENTS)"
	@echo "  make poc                           - roda a Prova de Conceito (Etapa 0, 100 imagens)"
	@echo "  make auditoria                     - roda a auditoria da base completa (Etapa 1)"
	@echo "  make dedup                         - roda a deduplicacao e analise de sensibilidade (Etapa 2)"
	@echo "  make all                            - roda auditoria e dedup, nessa ordem"
	@echo "  make clean-resultados CONFIRM=yes  - apaga os CSVs gerados em resultados/ (Etapas 1 e 2)"

venv:
	python -m venv "$(VENV_DIR)"

install:
	"$(PYTHON)" -m pip install -r "$(REQUIREMENTS)"

poc:
	"$(PYTHON)" "$(POC_SCRIPT)"

auditoria etapa1:
	"$(PYTHON)" "$(ETAPA1_SCRIPT)"

dedup deduplicacao etapa2:
	"$(PYTHON)" "$(ETAPA2_SCRIPT)"

all: auditoria dedup

clean-resultados:
	@test "$(CONFIRM)" = "yes" || { echo "Isso apaga os CSVs gerados em:"; echo "  $(ETAPA1_RESULTADOS)"; echo "  $(ETAPA2_RESULTADOS)"; echo "Rode de novo com: make clean-resultados CONFIRM=yes"; exit 1; }
	rm -rf "$(ETAPA1_RESULTADOS)" "$(ETAPA2_RESULTADOS)"
