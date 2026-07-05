CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -Iinclude -O2
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
RESULTS_DIR = results

COMMON_SRCS = $(SRC_DIR)/pbm.c $(SRC_DIR)/chaincode.c $(SRC_DIR)/ctn.c
COMMON_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(COMMON_SRCS))

.PHONY: all clean test

all: $(BIN_DIR)/codificador $(BIN_DIR)/decodificador $(RESULTS_DIR)

$(BIN_DIR)/codificador: $(COMMON_OBJS) $(BUILD_DIR)/encode.o | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/decodificador: $(COMMON_OBJS) $(BUILD_DIR)/decode.o | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Pasta onde ficam os arquivos gerados (.pbm/.ctn) ao rodar os programas,
# por exemplo via 'make test'. Criada automaticamente pelo 'make' e
# removida pelo 'make clean'.
$(RESULTS_DIR):
	mkdir -p $(RESULTS_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(RESULTS_DIR)

# Executa um teste rápido usando os exemplos da pasta examples/
test: all
	@echo "== Decodificando examples/exemplo.ctn (exemplo do enunciado) =="
	./$(BIN_DIR)/decodificador examples/exemplo.ctn $(RESULTS_DIR)/exemplo_decodificado.pbm
	@echo ""
	@echo "== Codificando examples/exemplo_forma.pbm =="
	./$(BIN_DIR)/codificador examples/exemplo_forma.pbm $(RESULTS_DIR)/exemplo_codificado.ctn
	@echo ""
	@echo "== Reciclando: decodificando de volta o .ctn gerado =="
	./$(BIN_DIR)/decodificador $(RESULTS_DIR)/exemplo_codificado.ctn $(RESULTS_DIR)/exemplo_reconstruido.pbm

# Regra de padrão: roda codificação + decodificação em qualquer imagem
# .pbm presente na pasta examples/, bastando chamar
# "make <nome_da_imagem>.pbm", por exemplo:
#
#   make estrela.pbm
#
# Isso gera, dentro de results/:
#   <nome_da_imagem>.ctn               (saída do codificador)
#   <nome_da_imagem>_reconstruido.pbm  (saída do decodificador, a partir do .ctn acima)

%.pbm: examples/%.pbm all
	@echo "== Codificando examples/$@ =="
	./$(BIN_DIR)/codificador examples/$@ $(RESULTS_DIR)/$*.ctn
	@echo ""
	@echo "== Decodificando de volta a partir de $(RESULTS_DIR)/$*.ctn =="
	./$(BIN_DIR)/decodificador $(RESULTS_DIR)/$*.ctn $(RESULTS_DIR)/$*_reconstruido.pbm
