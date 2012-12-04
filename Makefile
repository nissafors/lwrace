# Varibles
CC := gcc
PROGRAM := lwrace
CFLAGS := -Wall -g
LFLAGS := -lncurses -lm
VPATH := src
OBJECTS := main.o       \
           player.o     \
           enemies.o    \
           treasures.o  \
           miscfunc.o    \
           drawfigure.o \
           fobjects.o   \
           cline.o      \
           settings.o   \
           path.o       \
           score.o      \
           keys.o
# Cygwin hides curses.h, must specify include path:
INC_PATH := -I/usr/include/ncurses/

# Rule for building executable
$(PROGRAM): $(OBJECTS)
	$(CC) $(INC_PATH) $(CFLAGS) $^ $(LFLAGS) -o $(PROGRAM)

# Rule for building object files
%.o: %.c
	$(CC) -c $(INC_PATH) $(CFLAGS) $<

# Dependencies
main.o        \
player.o      \
enemies.o     \
treasures.o   \
drawfigure.o  \
fobjects.o    \
cline.o       \
settings.o    \
score.o: globals.h
main.o: main.h
enemies.o: enemies.h
fobjects.o: fobjects.h
cline.o: cline.h

# Install lwrace into /usr/local/bin
install:
	cp lwrace /usr/local/bin/lwrace
	
# Cleaning
clean:
	rm -f $(PROGRAM)
	rm -f *.o
	rm -f ~*
	rm -f *~
	rm -f *.gch
