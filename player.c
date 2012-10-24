#include <curses.h>
#include "lwrace.h"

/* Read keybuffer and return direction, pause or exit. "curdir" is current
 * direction, which is returned if none of the specified keys have been
 * pressed */
int getdir(int curdir) {
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
struct pos drawPlayer(int dir, struct pos curpos) {
	extern int Rows, Cols;
	static double lasttime;
	struct pos newpos;
	double delay = PLAYER_DELAY;

	newpos = curpos;
	if (setpos(dir, &newpos, &delay, &lasttime)) {
		if (newpos.row > Rows)              /* Player may be off screen if   */
			newpos.row = Rows - PL_ADJ_MARGIN;  /* screen size changed. Move */
		if (newpos.col > Cols)              /* inside with a margin to avoid */
			newpos.col = Cols - PL_ADJ_MARGIN;  /* instant death.            */
		if ((curpos.row == newpos.row && curpos.col == newpos.col 
				&& dir != INIT) || dir == STOP)
			return curpos;/* Don't redraw unless moved or need to initialize */
		mvaddch(curpos.row,curpos.col,BACKGROUND);      /* erase old player  */
		mvaddch(newpos.row,newpos.col,PLAYER);          /* draw new player   */
		curpos.row = newpos.row, curpos.col = newpos.col;/* remember position*/
		lasttime = getnow();
	}
	return curpos;
}
