BINCLIENT = client
BINSERVER = server
OBJS = $(BINCLIENT:=.o) $(BINSERVER:=.o) cliente_func.o  bbdd.o common.o

CFLAGS = -g -O2 -Wall -pedantic -ansi -w

all: $(BINCLIENT) $(BINSERVER)

client: 
	$(CC) $(CFLAGS) -o $(BINCLIENT) cliente.c cliente_func.c common.c

server:
	$(CC) $(CFLAGS) -o $(BINSERVER) servidor.c bbdd.c common.c

clean:	
	rm -rf *.log core *~ $(OBJS) $(BINCLIENT) $(BINSERVER)
	
cleancli:
	rm -rf *.log core *~ $(BINCLIENT) 

cleanser:
	rm -rf *.log core *~ $(BINSERVER)
	
