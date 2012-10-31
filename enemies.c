#include <curses.h>     /* For mvaddch() and mvprintw() */
#include <string.h>     /* For strlen() */
#include <stdlib.h>     /* For abs() */
#include "globals.h"
#include "enemies.h"

/* 
 * Draw enemies hunting player. Enemies are added at ADD_ENEMY_SCORE_INTERVAL
 * up to MAX_ENEMIES. Player will be immortal for IMMORTAL_TIME seconds every
 * time an enemy is added. The first enemy will need ENEMY_DELAY seconds to
 * cover the screen and each successor is ENEMY_DELAY_DIFF slower than its
 * predecessor.
 */
bool_t drawenemies(struct pos plpos, int score) {
	extern int    rows, cols; /* Current screen size */
	static struct pos lastpos[MAX_ENEMIES];
	static double lasttime[MAX_ENEMIES]; /* Used by setpos() for delay timer */
	static int    lastrows[MAX_ENEMIES], lastcols[MAX_ENEMIES];
	static double delaydiff[MAX_ENEMIES];
	static int    encount;
	static double immortaltimer;
	static bool_t displaytimer;
	struct pos    enpos[MAX_ENEMIES];
	double        rdelay[MAX_ENEMIES], cdelay[MAX_ENEMIES];
	double        timeleft;
	int           i, j;
	dir_t         dir = STOP;
	char          *immortaltext = "* Untouchable for %d seconds!";

	/* Add an enemy on certain score intervals */
	if ((float)score / (float)ADD_ENEMY_SCORE_INTERVAL == encount
	  && encount < MAX_ENEMIES) { 
		/* Randomize starting position for the new enemy */
		enpos[encount].row = genrand(0,rows);
		enpos[encount].col = genrand(0,cols);
		/* Every new enemy may have a different delay than the previous one as
		 * specified with ENEMY_DELAY_DIFF. The actual delay is calculated for
		 * each enemy below as it's dependent on current screen size */
		delaydiff[encount] = ENEMY_DELAY_DIFF * encount;
		/* Keep track of how many enemies there are */
		encount++;               
		/* Start timer for player immortality */
		immortaltimer = getnow();
		displaytimer = TRUE;
	}

	/* Find out where each enemy should be and draw it if the position has
	 * changed since last time */
	for (i = 0; i < encount; i++) {
		/* We need current and previous positions at the same time */
		enpos[i] = lastpos[i];
		/* Get new direction  */
		dir = hunt(&plpos, enpos+i, i);
		/* Calculate delays */
		rdelay[i] = ENEMY_DELAY_ROW / rows + delaydiff[i] / rows;
		cdelay[i] = ENEMY_DELAY_COL / cols + delaydiff[i] / cols;
		/* Did position change since last time? */
		if (setpos(dir, enpos+i, rdelay+i, cdelay+i, lasttime+i)) {
			/* Draw enemy */
			lastpos[i] = drawfigure(enpos[i], ENEMY, lastpos[i], BACKGROUND,
			                        lastrows[i], lastcols[i]); 
			/* Shorthand for immortality time left for player */
			timeleft = immortaltimer + IMMORTAL_TIME - getnow();
			/* Is player immortal? */
			if (timeleft < 0) {
				/* No! Is timer still visible and should be erased? */
				if (displaytimer == TRUE) {
					/* Yes. Remove it. */
					for (j = 0; j < strlen(immortaltext); j++) {
						mvaddch(rows, cols / 2 - strlen(immortaltext) / 2 + j, 
								BACKGROUND);
					}
					displaytimer = FALSE;
				}	
				/* Was player hit? */
				if (enpos[i].row == plpos.row && enpos[i].col == plpos.col)
					/* Yes. Return to caller */
					return HIT;
			} else { 
				/* Player is immortal: display timer */
				mvprintw(rows, cols/2-strlen(immortaltext)/2, immortaltext,
					(int)timeleft + 1);
			}
			/* remember screen size */
			lastrows[i] = rows, lastcols[i] = cols;
		}
	}
	/* Player survived this round */
	return MISS;
}

/* 
 * Supply logic and position updates for enemies. Return direction.
 */
dir_t hunt(struct pos *target, struct pos *hunter, int logic) {
	extern int    rows, cols; /* Current screen size */
	int up, down, left, right;
	up = down = left = right = FALSE;
	
	/* There are 3 kinds of logic. Make sure we don't use numbers above that */
	logic = logic % 3;

	/* Always use logic 0 if distance to player is less or more than specified
	 * by LOGIC_MAXDIST and LOGIC_MINDIST */
	if  (
			(
			 	(double)abs(hunter->row - target->row) 
				< LOGIC_MIN_ROWQ * (double)rows
				&& (double)abs(hunter->col - target->col)
				< LOGIC_MIN_COLQ * (double)cols
			)
			||  (double)abs(hunter->row - target->row)
				> LOGIC_MAX_ROWQ * (double)rows
			||  (double)abs(hunter->col - target->col)
				> LOGIC_MAX_COLQ * (double)cols
		)
		logic = 0;

	/* Apply logic */
	if (logic == 1) {
		if (hunter->row < target->row)
			down = TRUE;
		else if (hunter->row > target->row)
			up = TRUE;
		else if (hunter->col < target->col)
			right = TRUE;
		else if (hunter->col > target->col)
			left = TRUE;
	}
	if (logic == 2) {
		if (hunter->col < target->col)
			right = TRUE;
		else if (hunter->col > target->col)
			left = TRUE;
		else if (hunter->row < target->row)
			down = TRUE;
		else if (hunter->row > target->row)
			up = TRUE;
	}
	if (logic == 0) {
		if (hunter->row < target->row)
			down = TRUE;
		else if (hunter->row > target->row)
			up = TRUE;
		if (hunter->col < target->col)
			right = TRUE;
		else if (hunter->col > target->col)
			left = TRUE;
	}
	
	/* Return direction */
	if (up && right)
		return NE;
	else if (up && left)
		return NW;
	else if (down && right)
		return SE;
	else if (down && left)
		return SW;
	else if (up)
		return UP;
	else if (down)
		return DOWN;
	else if (right)
		return RIGHT;
	else if (left)
		return LEFT;
	else
		return STOP;
}
