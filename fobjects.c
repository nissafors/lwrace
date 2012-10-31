/* Part of Lawyer Race 
   Generate objects falling down on player
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
#include "fobjects.h"
#include "enemies.h" /* Needs to know MAX_ENEMIES */

/*
 * Generates dangerous falling objects at certain score intervals after the
 * last enemy has emerged.
 * Before an object starts falling it will hang from the ceiling for a random
 * time between "fobj_init_delay_min" and "fobj_init_delay_max". When it starts
 * falling the delay is defined by "fobj_fall_delay_start", and then accelerates
 * up to "fobj_fall_delay_end". The acceleration algorithm is a exponential
 * function where the base is "fobj_base" and the exponent is the portion of the
 * screen it has fallen multiplied by "fobj_acc". When the object has reached
 * the floor it returns to the ceiling with a new "hang" delay between
 * "fobj_hang_delay_min" and "fobj_hang_delay_max". All delays will be divided
 * by rows to adjust for screen size.
 * If the player was hit by an object the function returns HIT.
 * Otherwise it returns MISS.
 */
bool_t fobjects(struct pos plpos, int score) {
	extern int    rows, cols; /* Current screen size */
	extern int    fobj_init_delay_min, fobj_init_delay_max;
	extern int    fobj_hang_delay_min, fobj_hang_delay_max;
	extern double fobj_fall_delay_start, fobj_fall_delay_end;
	extern double fobj_base, fobj_acc;
	static struct pos lastpos[MAX_OBJECTS];
	static double lasttime[MAX_OBJECTS]; /* Used by setpos() for delay timer */
	static int    lastrows[MAX_OBJECTS], lastcols[MAX_OBJECTS];
	static int    objcount;
	struct pos    objpos[MAX_OBJECTS];
	static double delay[MAX_OBJECTS];
	int           i;

	/* Add an object on certain score intervals */
	if ((float)score / (float)ADD_OBJECTS_SCORE_INTERVAL == objcount +
	   MAX_ENEMIES && objcount < MAX_OBJECTS) { 
		/* Randomize starting position for the new object */
		lastpos[objcount].row = 0;
		lastpos[objcount].col = genrand(0,cols);
		/* Randomize initial delay: The time this object hangs from the
		 * ceiling before it starts falling the first time it appears */
		delay[objcount] = (float)genrand(fobj_init_delay_min * 100,
		                   fobj_init_delay_max * 100) / 100.0;;
		/* Adjust delay for screen size */
		delay[objcount] /= rows;
		/* Keep track of how many objects there are */
		objcount++;
	}

	/* Find out where each object should be and draw it if the position has
	 * changed since last time */
	for (i = 0; i < objcount; i++) {
		/* We need current and previous positions at the same time */
		objpos[i] = lastpos[i];
		/* Did position change since last time? */
		if (setpos(DOWN, objpos+i, delay+i, delay+i, lasttime+i)) {
			/* Calculate delay for next round */
			delay[i] = fobj_fall_delay_end + fobj_fall_delay_start *
			           pow(fobj_base, -((double)objpos[i].row / (double)rows) *
			           fobj_acc);
			/* If object fell through bottom att screen restart at top */
			if (lastpos[i].row > rows - 2) {
				objpos[i].row = 0;
				objpos[i].col = genrand(0, cols);
				/* Set new delay for how long it should hang from the ceiling.*/
				delay[i] = (float)genrand(fobj_hang_delay_min * 100,
				             fobj_hang_delay_max * 100) / 100.0;
			}
			/* Draw object */
			lastpos[i] = drawfigure(objpos[i], OBJECT, lastpos[i], BACKGROUND,
			                        lastrows[i], lastcols[i]); 
			/* Adjust delay for screen size */
			delay[i] /= rows;
			/* remember screen size */
			lastrows[i] = rows, lastcols[i] = cols;
		}
		/* Was player hit? */
		if (objpos[i].row == plpos.row && objpos[i].col == plpos.col)
			return HIT; /* Yes. Return to caller */
	}

	/* Player survived this round */
	return MISS;
}
