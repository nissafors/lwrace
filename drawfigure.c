#include <curses.h>
#include "globals.h"

/*
 * Draw figure drawc at drawpos and erase (using erasec) at erasepos.
 * Screensize at last call should be passed as oldrows, oldcols.
 * If drawpos = erasepos nothing will be erased.
 */
struct pos drawfigure (struct pos drawpos, char drawc,
                       struct pos erasepos, char erasec,
                       int oldrows, int oldcols) {
	extern int rows, cols;

	/* If screen size has changed and figure is outside, move inside */
	if (rows != oldrows && oldrows != 0)
		drawpos.row *= (float)rows / (float)oldrows;
	if (cols != oldcols && oldcols != 0)
		drawpos.col *= (float)cols / (float)oldcols;

	/* Erase old and draw new enemy. */
	mvaddch(erasepos.row, erasepos.col, erasec); /* Erase    */
	mvaddch(drawpos.row, drawpos.col, drawc);    /* Draw     */

	/* Return new position */
	return drawpos;
}
