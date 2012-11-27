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

main.o: main.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $<
player.o: player.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $<
enemies.o: enemies.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $<
treasures.o: treasures.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $<
getnow.o: getnow.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $<
genrand.o: genrand.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $<
drawfigure.o: drawfigure.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $<
fobjects.o: fobjects.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $<
cline.o: cline.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $<
settings.o: settings.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $<
path.o: path.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $<
score.o: score.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $<

main.o player.o enemies.o treasures.o drawfigure.o fobjects.o cline.o settings.o score.o: globals.h
main.o: main.h
enemies.o: enemies.h
fobjects.o: fobjects.h
cline.o: cline.h

clean:
	rm -f $(PROGRAM)
	rm -f *.o
	rm -f ~*
	rm -f *~
	rm -f *.gch
