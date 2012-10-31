#include <curses.h>
#include <math.h>
#include "globals.h"
#include "fobjects.h"
#include "enemies.h" /* Needs to know MAX_ENEMIES */

/*
 * Generates dangerous falling objects at certain score intervals after the
 * last enemy has emerged. Intervals and delays are controlled by defines in
 * fobjects.h.
 * If the player was hit by an object the function returns HIT.
 * Otherwise it returns MISS.
 */
bool_t fobjects(struct pos plpos, int score) {
	extern int    rows, cols; /* Current screen size */
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
		delay[objcount] = (float)genrand(FOBJ_INIT_DELAY_MIN * 100,
		                   FOBJ_INIT_DELAY_MAX * 100) / 100.0;;
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
			delay[i] = FOBJ_FALL_DELAY_END + FOBJ_FALL_DELAY_START *
			           pow(FOBJ_BASE, -((float)objpos[i].row / (float)rows) *
			           FOBJ_ACC);
			/* If object fell through bottom att screen restart at top */
			if (lastpos[i].row > rows - 2) {
				objpos[i].row = 0;
				objpos[i].col = genrand(0, cols);
				/* Set new delay for how long it should hang from the ceiling.*/
				delay[i] = (float)genrand(FOBJ_HANG_DELAY_MIN * 100,
				             FOBJ_HANG_DELAY_MAX * 100) / 100.0;
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
			return MISS; /* Yes. Return to caller */
	}

	/* Player survived this round */
	return MISS;
}
