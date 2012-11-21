CC := gcc
PROGRAM := lwrace
CFLAGS := -Wall -g
LFLAGS := -lncurses -lm
VPATH := src
OBJECTS := main.o player.o enemies.o treasures.o getnow.o genrand.o drawfigure.o \
fobjects.o cline.o settings.o path.o score.o
# Cygwin hides curses.h, must specify include path:
#ifeq ($(shell uname), Cygwin)
INC_PATH := -I/usr/include/ncurses/
#endif

$(PROGRAM): $(OBJECTS)
	$(CC) $(INC_PATH) $(CFLAGS) $^ $(LFLAGS) -o $(PROGRAM)

main.o: main.c main.h globals.h
	$(CC) -c $(INC_PATH) $(CFLAGS) $^
player.o: player.c globals.h
	$(CC) -c $(INC_PATH) $(CFLAGS) $^
enemies.o: enemies.c enemies.h globals.h
	$(CC) -c $(INC_PATH) $(CFLAGS) $^
treasures.o: treasures.c globals.h
	$(CC) -c $(INC_PATH) $(CFLAGS) $^
getnow.o: getnow.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $^
genrand.o: genrand.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $^
drawfigure.o: drawfigure.c globals.h
	$(CC) -c $(INC_PATH) $(CFLAGS) $^
fobjects.o: fobjects.c fobjects.h globals.h
	$(CC) -c $(INC_PATH) $(CFLAGS) $^
cline.o: cline.c cline.h globals.h
	$(CC) -c $(INC_PATH) $(CFLAGS) $^
settings.o: settings.c globals.h
	$(CC) -c $(INC_PATH) $(CFLAGS) $^
path.o: path.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $^
score.o: score.c globals.h
	$(CC) -c $(INC_PATH) $(CFLAGS) $^

clean:
	rm -f $(PROGRAM)
	rm -f *.o
	rm -f ~*
	rm -f *~
