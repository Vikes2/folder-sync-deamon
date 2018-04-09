IDIR =../src
CC=gcc
CFLAGS=-I $(IDIR)

ODIR=../obj
LDIR =lib

LIBS=-lm

_DEPS = list.h sync.h copy.h util.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o list.o sync.o copy.o util.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

sync: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 