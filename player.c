#include <curses.h>
#include "lwrace.h"

/* Read keybuffer and return direction, pause or exit. "curdir" is current
 * direction, which is returned if none of the specified keys have been
 * pressed */
dir_t getdir(dir_t curdir) {
	int key = getch();

	switch (key) {
		case K_UP:
			return UP;
		case K_DOWN:
			return DOWN;
		case K_LEFT:
			return LEFT;
		case K_RIGHT:
			return RIGHT;
		case K_STOP:
			return STOP;
		case K_PAUSE:
			return PAUSE;
		case K_ESC:
			return EXIT;
	}
	return curdir;  /* No valid keys pressed, return current direction */
}

/* Draw player. "dir" is the direction in which player is moving. Return
 * players new position. */
struct pos drawplayer(dir_t dir, struct pos curpos) {
	extern int    rows, cols;
	static double lasttime;           /* Used by setpos() for delay timer */
	static int    lastrows, lastcols; /* used by drawfigure() */
	struct pos    oldpos;
	double        rdelay = PLAYER_DELAY_ROW / rows;
	double        cdelay = PLAYER_DELAY_COL / cols;

	/* Set new position and draw player */
	oldpos = curpos;
	if (setpos(dir, &curpos, &rdelay, &cdelay, &lasttime) || dir == INIT) {
		curpos = drawfigure(curpos, PLAYER, oldpos, BACKGROUND,
		                    lastrows, lastcols);
		lastrows = rows, lastcols = cols; /* Remember screen size */
	}
	/* Go home */
	return curpos;
}
