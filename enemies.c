#include <curses.h>     /* For mvaddch() and mvprintw() */
#include <string.h>     /* For strlen() */
#include <stdlib.h>     /* For abs() */
#include "lwrace.h"

/* Draw enemies hunting player. Enemies are added at ADD_ENEMY_SCORE_INTERVAL
 * up to MAX_ENEMIES. Player will be immortal for IMMORTAL_TIME seconds every
 * time an enemy is added. The first enemy will move every ENEMY_DELAY seconds,
 * and each successor is ENEMY_DELAY_DIFF slower than the last one.
 */
int drawEnemies(struct pos plpos, int score) {
	extern int    Rows, Cols; /* Current screen size */
	static struct pos enpos[MAX_ENEMIES], lastpos[MAX_ENEMIES];
	static double lasttime[MAX_ENEMIES], delay[MAX_ENEMIES];
	static int    enCount;
	static double immortaltimer;
	double        timeleft;
	int           i, j;
	dir_t         dir = STOP;
	char          *immortaltext = "* Untouchable for %d seconds!";

	/* Add an enemy on certain score intervals */
	if ((float)score / (float)ADD_ENEMY_SCORE_INTERVAL == enCount
	  && enCount < MAX_ENEMIES) { 
		enpos[enCount].row = genrand(0,Rows); /* Randomize starting pos for  */
		enpos[enCount].col = genrand(0,Cols); /* the new enemy               */
		delay[enCount] = ENEMY_DELAY + ENEMY_DELAY_DIFF * enCount; /* Set delay.
                          Every new enemy may have a different delay than the
                          previous one as specified with ENEMY_DELAY_DIFF*/
		enCount++;               /* Keep track of how many enemies there are */
		immortaltimer = getnow();
	}

	/* Find out where each enemy should be and draw it if the position has
	 * changed since last time */
	for (i = 0; i < enCount; i++) {
		dir = hunt(&plpos, enpos+i, i);         /* Get new direction  */
		if (setpos(dir, enpos+i, delay+i, lasttime+i)) {
			/* If screen size has changed and enemies is outside, move inside */
			if (enpos[i].row > Rows) enpos[i].row = Rows - 1;
			if (enpos[i].col > Cols) enpos[i].col = Cols - 1;
			mvaddch(lastpos[i].row, lastpos[i].col, BACKGROUND);  /* Erase    */
			mvaddch(enpos[i].row, enpos[i].col, ENEMY);           /* Draw     */
			lastpos[i].row = enpos[i].row;
			lastpos[i].col = enpos[i].col;              /* remember position  */
			lasttime[i] = getnow();
			if (immortaltimer + IMMORTAL_TIME < getnow()) {
				if (enpos[i].row == plpos.row &&    /* Was player killed? */
					enpos[i].col == plpos.col)
						return HIT;
			} else {    /* Player is immortal: display timer */
				timeleft = immortaltimer + IMMORTAL_TIME - getnow();
				mvprintw(Rows, Cols/2-strlen(immortaltext)/2, immortaltext,
					(int)timeleft + 1);
			if (timeleft < 0.1)   /* Erase timer just before time runs out */
				for (j = 0; j < strlen(immortaltext); j++)
					mvaddch(Rows, Cols/2-strlen(immortaltext)/2+j, BACKGROUND);
			}
		}
	}
	return MISS;
}

/* Supply logic and position updates for enemies. Return direction. */
dir_t hunt(struct pos *target, struct pos *hunter, int logic) {
	extern int    Rows, Cols; /* Current screen size */
	int up, down, left, right;
	up = down = left = right = FALSE;
	
	/* There are 3 kinds of logic. Make sure we don't use numbers above that */
	logic = logic % 3;

	/* Always use logic 0 if distance to player is less or more than specified
	 * by LOGIC_MAXDIST and LOGIC_MINDIST */
	if  (
			(
			 	(double)abs(hunter->row - target->row) 
				< LOGIC_MIN_ROWQ * (double)Rows
				&& (double)abs(hunter->col - target->col)
				< LOGIC_MIN_COLQ * (double)Cols
			)
			||  (double)abs(hunter->row - target->row)
				> LOGIC_MAX_ROWQ * (double)Rows
			||  (double)abs(hunter->col - target->col)
				> LOGIC_MAX_COLQ * (double)Cols
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
