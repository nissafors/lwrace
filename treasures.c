#include <curses.h>
#include "lwrace.h"

/* Add a treasure to screen that player should try to get. Return HIT if he
 * catches one, return MISS if not. */
bool_t treasures(struct pos plpos) {
	extern int rows, cols;
	static struct pos trpos;
	static int trcount;
	static int oldrows, oldcols;

	if (!trcount) {                 /* New treasure needed */
		trpos.row = genrand(0, rows);
		trpos.col = genrand(0, cols);
		trcount++;
	}
  
	/* Draw treasure */
	trpos = drawfigure(trpos, TREASURE, trpos, BACKGROUND, oldrows, oldcols);
	/* Remember screensize */
	oldrows = rows, oldcols = cols;
	if (plpos.row == trpos.row && plpos.col == trpos.col) { /* Player hit it */
		trcount--;
		mvaddch(trpos.row, trpos.col, PLAYER); /* Use player char to erase old
                                               treasure, to cover the situation
                                               where player stopped on top of
                                               treasure */
		return HIT;
	}
	return MISS;
}
