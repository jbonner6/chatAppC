IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) -Wall -g -pthread

ODIR=obj
SDIR=src
LDIR =../lib

_DEPS =
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_COBJ = chatClient.o
COBJ = $(patsubst %,$(ODIR)/%,$(_COBJ))

_SOBJ = chatServer.o
SOBJ = $(patsubst %,$(ODIR)/%,$(_SOBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

makeAll: clientMake serverMake

clientMake: $(COBJ)
	gcc -o $@ $^ $(CFLAGS)
serverMake: $(SOBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
