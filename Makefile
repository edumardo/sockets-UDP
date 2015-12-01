#################################################
# Definitions & variables
#
BINCLIENT = client
BINSERVER = server
OBJSCLIENT = $(BINCLIENT:=.o) client_func.o common.o
OBJSSERVER = $(BINSERVER:=.o) server_func.o common.o database.o

CFLAGS = -g -O2 -Wall -pedantic -ansi -w -std=c99

#################################################
# All section
###
all: $(BINCLIENT) $(BINSERVER)

#################################################
# Client section
###
$(BINCLIENT): $(OBJSCLIENT)
	$(CC) $(CFLAGS) -o $(BINCLIENT) $(OBJSCLIENT)
	
client.o: client.c
	$(CC) $(CFLAGS) -c client.c

client_func.o: client_func.c
	$(CC) $(CFLAGS) -c client_func.c

#################################################
# Server section
###
$(BINSERVER): $(OBJSSERVER)
	$(CC) $(CFLAGS) -o $(BINSERVER) $(OBJSSERVER)

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

server_func.o: server_func.c
	$(CC) $(CFLAGS) -c server_func.c

database.o: database.c
	$(CC) $(CFLAGS) -c database.c

#################################################
# Common section
###
common.o: common.c
	$(CC) $(CFLAGS) -c common.c
	
#################################################
# Clean section
###
clean:
	rm -rf *.log *~ $(BINCLIENT) $(BINSERVER) $(OBJSCLIENT) $(OBJSSERVER)

cleancli:
	rm -rf *.log core *~ $(BINCLIENT) $(OBJSCLIENT)

cleanser:
	rm -rf *.log core *~ $(BINSERVER) $(OBJSSERVER)
