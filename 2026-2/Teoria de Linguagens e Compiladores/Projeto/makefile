SRC   = src
BUILD = build
BIN   = bin
EXE   = $(BIN)/simples

$(EXE): $(SRC)/lexico.l $(SRC)/sintatico.y $(SRC)/utils.c
	mkdir -p $(BUILD) $(BIN)
	flex  -o $(BUILD)/lexico.c $(SRC)/lexico.l
	bison -o $(BUILD)/sintatico.c $(SRC)/sintatico.y -v -d
	gcc $(BUILD)/sintatico.c -I$(SRC) -o $(EXE)
	gcc $(BUILD)/mvs.c -I$(SRC) -o $(EXE)

teste: $(EXE)
	@for f in testes/validos/*.simples; do \
	    n=$$(basename $$f .simples); \
	    ./$(EXE) testes/validos/$$n > /dev/null; \
	    if diff -q testes/validos/$$n.mvs testes/esperados/$$n.mvs > /dev/null; \
	    then echo "$$n OK"; else echo "$$n FALHOU"; fi; \
	done

limpa:
	rm -f $(BUILD)/* $(EXE) testes/validos/*.mvs testes/invalidos/*.mvs

.PHONY: teste limpa