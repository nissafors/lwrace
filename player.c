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
	extern int rows, cols;
	static double lasttime;     /* Used by setpos() for delay timer */
	struct pos newpos;
	double rdelay = PLAYER_DELAY_ROW / rows;
	double cdelay = PLAYER_DELAY_COL / cols;

	/* Set new position and draw player */
	newpos = curpos;
	if (setpos(dir, &newpos, &rdelay, &cdelay, &lasttime)) {
		if (newpos.row > rows)              /* Player may be off screen if   */
			newpos.row = rows - PL_ADJ_MARGIN;  /* screen size changed. Move */
		if (newpos.col > cols)              /* inside with a margin to avoid */
			newpos.col = cols - PL_ADJ_MARGIN;  /* instant death.            */
		if ((curpos.row == newpos.row && curpos.col == newpos.col 
				&& dir != INIT) || dir == STOP)
			return curpos;/* Don't redraw unless moved or need to initialize */
		mvaddch(curpos.row,curpos.col,BACKGROUND);      /* erase old player  */
		mvaddch(newpos.row,newpos.col,PLAYER);          /* draw new player   */
		curpos.row = newpos.row, curpos.col = newpos.col;/* remember position*/
	}
	return curpos;
}
