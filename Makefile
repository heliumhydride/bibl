include config.mk

all: o/bibl

bibl.o: bibl.c
	$(CC) -c $(CFLAGS) -o bibl.o bibl.c

prettyprint.o: prettyprint.c
	$(CC) -c $(CFLAGS) -o prettyprint.o prettyprint.c

# also link with curses
o/bibl: bibl.o prettyprint.o
	$(CC) $(CFLAGS) -o o/bibl prettyprint.o bibl.o -lcurses

clean:
	rm -rf *.o examples/*.o *.so* *.a o/*

.PHONY: clean examples shared static