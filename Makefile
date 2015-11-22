BINCLIENT = client
BINSERVER = server
OBJS = $(BINCLIENT:=.o) $(BINSERVER:=.o) client_func.o server_func.o database.o common.o

CFLAGS = -g -O2 -Wall -pedantic -ansi -w

all: $(BINCLIENT) $(BINSERVER)

client: 
	$(CC) $(CFLAGS) -o $(BINCLIENT) client.c client_func.c common.c

server:
	$(CC) $(CFLAGS) -o $(BINSERVER) server.c server_func.c database.c common.c

clean:	
	rm -rf *.log core *~ $(OBJS) $(BINCLIENT) $(BINSERVER)
	
cleancli:
	rm -rf *.log core *~ $(BINCLIENT) 

cleanser:
	rm -rf *.log core *~ $(BINSERVER)
	
