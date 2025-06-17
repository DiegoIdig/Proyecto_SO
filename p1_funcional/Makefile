# Definir el compilador
CC = gcc
CFLAGS = -Wall -O2

OBJS = $(patsubst %.c,%.o,$(wildcard *.c))

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Para limpiar los archivos generados
clean:
	rm -f *.o main
