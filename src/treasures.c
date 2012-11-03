/* Part of Lawyer Race 
   Add a treasure to screen that player should try to get. Return HIT if he
   catches one, return MISS if not.
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
#include "globals.h"

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
