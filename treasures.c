#include <curses.h>
#include "lwrace.h"

/* Add a treasure to screen that player should try to get. Return 1 if he
 * catches one, return 0 if not. */
int treasures(struct pos plpos) {
	extern int rows, cols;
	static struct pos trpos;
	static int trcount;

	if (!trcount) {                 /* New treasure needed */
		trpos.row = genrand(0, rows);
		trpos.col = genrand(0, cols);
		trcount++;
	}
  
	if (trpos.row > rows) trpos.row = rows - 1; /* If screen size has changed */
	if (trpos.col > cols) trpos.col = cols - 1; /* treasure may be unreachable*/

	mvaddch(trpos.row, trpos.col, TREASURE);  /* Draw outside if's, in case
                                                 enemy erases it */
	if (plpos.row == trpos.row && plpos.col == trpos.col) { /* Player hit it */
		trcount--;
		mvaddch(trpos.row, trpos.col, PLAYER); /* Use player char to erase old
                                               treasure, to cover the situation
                                               where player stopped on top of
                                               treasure */
		return 1;
	}
	return 0;
}
