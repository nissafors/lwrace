CC := gcc
PROGRAM := lwrace
CFLAGS := -Wall -g
LFLAGS := -lncurses -lm
OBJECTS := main.o player.o enemies.o treasures.o getnow.o genrand.o setpos.o drawfigure.o fobjects.o
# Cygwin hides curses.h, must specify include path:
ifeq ($(shell uname), Cygwin)
	INC_PATH := -I/usr/include/ncurses/
endif

$(PROGRAM): $(OBJECTS)
	$(CC) $(INC_PATH) $(CFLAGS) $^ $(LFLAGS) -o $(PROGRAM)

main.o: main.h globals.h
player.o: player.h globals.h
enemies.o: enemies.h globals.h
treasures.o: globals.h
setpos.o: globals.h
drawfigure.o: globals.h
fobjects.o: fobjects.h globals.h

clean:
	rm -f $(PROGRAM)
	rm -f *.o
	rm -f ~*
	rm -f *~
