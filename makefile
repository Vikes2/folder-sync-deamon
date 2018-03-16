IDIR =src
CC=gcc
CFLAGS=-I $(IDIR)

ODIR=obj
LDIR =lib

LIBS=-lm

_DEPS = list.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = list.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sync: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 