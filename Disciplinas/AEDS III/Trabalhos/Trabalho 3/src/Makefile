CC = gcc
CFLAGS = -Wall -Wextra -O3
LDFLAGS = -lm

TARGET = base

SRCS = base.c algoritmos.c
OBJS = $(SRCS:.c=.o)
HEADERS = algoritmos.h

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# -----------------------
# EXECUÇÃO NORMAL
# -----------------------
run:
	./$(TARGET) $(FILE)

# -----------------------
# TESTES AUTOMÁTICOS
# -----------------------
test:
	./run_all.sh

clean:
	rm -f $(OBJS) $(TARGET) results.txt