CC = gcc 
BINCLI = cliente
BINSER = servidor
OBJS = $(BIN:=.o) cliente_func.o  bbdd.o common.o

CFLAGS = -g -O2 -Wall -pedantic -ansi -w

all: $(BINCLI) $(BINSER)

cliente: 
	$(CC) $(CFLAGS) -o cliente cliente.c cliente_func.c common.c

servidor:
	$(CC) $(CFLAGS) -o servidor servidor.c bbdd.c common.c

clean:	
	rm -rf *.log core *~ $(OBJS) $(BINCLI) $(BINSER)
	
cleancli:
	rm -rf *.log core *~ $(BINCLI) 

cleanser:
	rm -rf *.log core *~ $(BINSER)
	
