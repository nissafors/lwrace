CC := gcc
PROGRAM := lwrace
CFLAGS := -Wall -g
LFLAGS := -lncurses
OBJECTS := main.o player.o enemies.o treasures.o getnow.o genrand.o setpos.o
# Cygwin hides curses.h, must specify include path:
ifeq ($(shell uname), Cygwin)
	INC_PATH := -I/usr/include/ncurses/
endif

$(PROGRAM): $(OBJECTS)
	$(CC) $(INC_PATH) $(CFLAGS) $^ $(LFLAGS) -o $(PROGRAM)

main.o: lwrace.h
player.o: lwrace.h
enemies.o: lwrace.h
treasures.o: lwrace.h
setpos.o: lwrace.h

clean:
	rm -f $(PROGRAM)
	rm -f *.o
	rm -f ~*
	rm -f *~
