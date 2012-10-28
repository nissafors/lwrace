#include <curses.h>
#include <math.h>
#include "lwrace.h"

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
	if ((float)score / (float)ADD_OBJECTS_SCORE_INTERVAL == objcount + MAX_ENEMIES
	  && objcount < MAX_OBJECTS) { 
		/* Randomize starting position for the new object */
		lastpos[objcount].row = 0;
		lastpos[objcount].col = genrand(0,cols);
		delay[objcount] = 0.1;
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
			/* Calculate delays */
			delay[i] = 0.05 * pow(2, -(objpos[i].row / rows) * 16);
			/* If object fell through bottom att screen restart at top */
			if (lastpos[i].row > rows - 2) {
				objpos[i].row = 0;
				objpos[i].col = genrand(0, cols);
				delay[i] = 0.8;
			}
			/* Draw object */
			lastpos[i] = drawfigure(objpos[i], OBJECT, lastpos[i], BACKGROUND,
			                        lastrows[i], lastcols[i]); 
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
