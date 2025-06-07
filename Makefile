CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm

SRCS = src/common.c src/program1.c src/program2.c src/program3.c
OBJS = $(SRCS:.c=.o)
TARGETS = organizar consultar gerar_teste

all: $(TARGETS)

organizar: src/common.o src/program1.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

consultar: src/common.o src/program2.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

gerar_teste: src/common.o src/program3.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGETS) *.txt

.PHONY: all clean 