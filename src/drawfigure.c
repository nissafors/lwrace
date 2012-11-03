/* Part of Lawyer Race 
   Functions to position and draw moving figures in the game
   Copyright (C) 2012 Andreas Andersson

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <curses.h>
#include <math.h>
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

/* 
 * Set pos based on dir if the time specified by *rd and *cd (row delay and
 * col delay) has passed since trec. Returns TRUE if position was updated
 * and resets *trec. Returns FALSE otherwise.
 */
bool_t setpos(dir_t dir, struct pos *rc, double *rd, double *cd, double *trec) {
	extern int rows, cols;
	double dd = sqrt(pow(*rd, 2) + pow(*cd, 2));

	switch (dir) {
		case UP:
			if (getnow() - *trec < *rd)
				return FALSE;
			rc->row--;
			break;
		case DOWN:
			if (getnow() - *trec < *rd)
				return FALSE;
			rc->row++;
			break;
		case RIGHT:
			if (getnow() - *trec < *cd)
				return FALSE;
			rc->col++;
			break;
		case LEFT:
			if (getnow() - *trec < *cd)
				return FALSE;
			rc->col--;
			break;
		case NE:
			if (getnow() - *trec < dd)
				return FALSE;
			rc->row--;
			rc->col++;
			break;
		case NW:
			if (getnow() - *trec < dd)
				return FALSE;
			rc->row--;
			rc->col--;
			break;
		case SE:
			if (getnow() - *trec < dd)
				return FALSE;
			rc->row++;
			rc->col++;
			break;
		case SW:
			if (getnow() - *trec < dd)
				return FALSE;
			rc->row++;
			rc->col--;
			break;
		default:
			return FALSE;   /* Position not changed */
	}

	/* Check if new position is outside screen */
	if (rc->row < 0)
		rc->row = 0;
	if (rc->row >= rows - 1)
		rc->row = rows - 1;
	if (rc->col < 0)
		rc->col = 0;
	if (rc->col >= cols - 1)
		rc->col = cols - 1;

	/* Reset timer and return to caller */
	*trec = getnow();
	return TRUE;
}
