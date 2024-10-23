include config.mk

all: o/bibl

bibl.o: bibl.c
	$(CC) -c $(CFLAGS) -o bibl.o bibl.c

prettyprint.o: prettyprint.c
	$(CC) -c $(CFLAGS) -o prettyprint.o prettyprint.c

# also link with curses, menu and panel
o/bibl: bibl.o prettyprint.o
	$(CC) $(CFLAGS) -o o/bibl prettyprint.o bibl.o -lcurses -lmenu -lpanel

clean:
	rm -rf *.o o/*

.PHONY: clean
