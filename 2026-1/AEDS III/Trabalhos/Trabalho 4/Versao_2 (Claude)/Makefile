CC     = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lm

SRC    = $(filter-out src/instancias.c, $(wildcard src/*.c))
TARGET    = bin/main
GENERATOR = bin/instancias

VENV      = venv
PY        = $(VENV)/bin/python
PIP       = $(VENV)/bin/pip
REQUIREMENTS = requirements.txt

# ==========================================================
# DEFAULT
# ==========================================================
all: $(TARGET)

# ==========================================================
# VENV + DEPENDÊNCIAS
# ==========================================================
setup:
	@echo "Verificando ambiente virtual..."
	@if [ ! -d "$(VENV)" ]; then \
		echo "Criando venv..."; \
		python3 -m venv "$(VENV)"; \
	fi
	@$(VENV)/bin/python -m pip install --upgrade pip -q
	@$(VENV)/bin/python -m pip install -r $(REQUIREMENTS) -q
	@echo "Dependencias OK."

# ==========================================================
# COMPILAÇÃO PRINCIPAL
# ==========================================================
$(TARGET): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# ==========================================================
# GERADOR DE INSTÂNCIAS
# ==========================================================
$(GENERATOR): src/instancias.c
	@mkdir -p bin
	$(CC) $(CFLAGS) src/instancias.c -o $(GENERATOR) $(LDFLAGS)

instancias: $(GENERATOR)
	./$(GENERATOR)

# ==========================================================
# EXECUÇÃO
# ==========================================================
run: $(TARGET)
	./run_prompt.sh

# ==========================================================
# PLOT
# ==========================================================
plot: setup
	@echo "Gerando graficos..."
	@mkdir -p Results/graficos
	$(PY) plot.py

# ==========================================================
# LIMPEZA
# ==========================================================
clean:
	rm -rf bin instances Results

clean-all:
	rm -rf bin instances Results venv

rebuild: clean all
