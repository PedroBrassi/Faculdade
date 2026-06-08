CC = gcc

CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lm

SRC = $(filter-out src/instancias.c, $(wildcard src/*.c))

TARGET = bin/main
GENERATOR = bin/instancias

VENV = venv
VENV_PATH = "$(abspath $(VENV))"
PY = $(VENV_PATH)/bin/python
PIP = $(VENV_PATH)/bin/pip

REQUIREMENTS = requirements.txt

# ==========================================================
# DEFAULT
# ==========================================================
all: setup instancias run plot

# ==========================================================
# VENV + DEPENDÊNCIAS (FORÇA CONSISTÊNCIA)
# ==========================================================
setup:
	@echo "Verificando ambiente virtual..."
	@if [ ! -d "$(VENV)" ]; then \
		echo "Criando venv..."; \
		python3 -m venv "$(VENV)"; \
	fi

	@echo "Garantindo pip atualizado..."
	"$(VENV)/bin/python" -m ensurepip --upgrade || true
	"$(VENV)/bin/python" -m pip install --upgrade pip

	@echo "Instalando dependências..."
	"$(VENV)/bin/python" -m pip install -r requirements.txt

# ==========================================================
# COMPILAÇÃO
# ==========================================================
$(TARGET): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./run_prompt.sh

# ==========================================================
# GERADOR
# ==========================================================
instancias: $(GENERATOR)
	./$(GENERATOR)

$(GENERATOR): src/instancias.c
	@mkdir -p bin
	$(CC) $(CFLAGS) src/instancias.c -o $(GENERATOR) $(LDFLAGS)

# ==========================================================
# PLOT (FORÇA VENV SEMPRE)
# ==========================================================
plot: setup
	@echo "Gerando gráficos com venv..."
	@mkdir -p Results/graficos
	$(PY) plot.py

# ==========================================================
# LIMPEZA
# ==========================================================
clean:
	@echo "Limpando projeto (sem apagar venv)..."
	rm -rf bin instances Results

clean-all:
	rm -rf bin instances Results venv

rebuild: clean all