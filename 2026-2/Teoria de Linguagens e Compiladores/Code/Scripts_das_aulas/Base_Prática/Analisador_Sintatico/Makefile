CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
CPPFLAGS =
LDFLAGS =
LDLIBS =e
EXEEXT =
ifeq ($(OS),Windows_NT)
EXEEXT = .exe
endif
TARGET = simples$(EXEEXT)
INPUT = teste.simples

.PHONY: all run clean limpa
all: $(TARGET)

$(TARGET): lexico.o sintatico.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Grouped targets require GNU Make 4.3 or newer and are safe with make -j.
sintatico.c sintatico.h sintatico.output &: sintatico.y
	$(BISON) -Wall -Werror=conflicts-sr -Werror=conflicts-rr -v -d -o sintatico.c $<

lexico.c: lexico.l sintatico.h
	$(FLEX) -o $@ $<

lexico.o: lexico.c sintatico.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

sintatico.o: sintatico.c sintatico.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET) "$(INPUT)"

clean:
	$(RM) simples simples.exe lexico.o sintatico.o lexico.c sintatico.c sintatico.h sintatico.output

limpa: clean
