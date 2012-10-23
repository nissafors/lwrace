#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include "lwrace.h"

/* Draw enemies hunting player. Enemies are added at ADD_ENEMY_SCORE_INTERVAL
 * up to MAX_ENEMIES. Player will be immortal for IMMORTAL_TIME seconds every
 * time an enemy is added. The first enemy will move every ENEMY_DELAY seconds,
 * and each successor is ENEMY_DELAY_DIFF slower than the last one.
 */
int drawEnemies(struct pos playerPos, int score) {
	extern int    Rows, Cols; /* Current screen size */
	static struct pos enPos[MAX_ENEMIES], lastPos[MAX_ENEMIES];
	static double lasttime[MAX_ENEMIES], delay[MAX_ENEMIES];
	static int    enCount;
	static double immortaltimer;
	double        timeleft;
	int           i, j;
	char          *immortaltext = "* Untouchable for %d seconds!";

	/* Add an enemy on certain score intervals */
	if ((float)score / (float)ADD_ENEMY_SCORE_INTERVAL == enCount
	  && enCount < MAX_ENEMIES) { 
		enPos[enCount].row = genrand(0,Rows); /* Randomize starting pos for  */
		enPos[enCount].col = genrand(0,Cols); /* the new enemy               */
		delay[enCount] = ENEMY_DELAY + ENEMY_DELAY_DIFF * enCount; /* Every new
                              enemy is a little slower than the previous one */
		enCount++;               /* Keep track of how many enemies there are */
		immortaltimer = getnow();
	}

	for (i = 0; i < enCount; i++) {
		if (getnow() - lasttime[i] > delay[i]) {
			/* If screen size has changed and enemies is outside, move inside */
			if (enPos[i].row > Rows) enPos[i].row = Rows - 1;
			if (enPos[i].col > Cols) enPos[i].col = Cols - 1;
			hunt(&playerPos, enPos+i, i);               /* Get new position   */
			mvaddch(lastPos[i].row, lastPos[i].col, BACKGROUND);  /* Erase    */
			mvaddch(enPos[i].row, enPos[i].col, ENEMY);           /* Draw     */
			lastPos[i].row = enPos[i].row;
			lastPos[i].col = enPos[i].col;              /* remember position  */
			lasttime[i] = getnow();
			if (immortaltimer + IMMORTAL_TIME < getnow()) {
				if (enPos[i].row == playerPos.row &&    /* Was player killed? */
					enPos[i].col == playerPos.col)
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

/* Supply logic and position updates for enemies */
void hunt(struct pos *target, struct pos *hunter, int logic) {
	extern int    Rows, Cols; /* Current screen size */
	
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

	if (logic == 1) {
		if (hunter->row < target->row)
			hunter->row++;
		else if (hunter->row > target->row)
			hunter->row--;
		else if (hunter->col < target->col)
			hunter->col++;
		else if (hunter->col > target->col)
			hunter->col--;
	}
	if (logic == 2) {
		if (hunter->col < target->col)
			hunter->col++;
		else if (hunter->col > target->col)
			hunter->col--;
		else if (hunter->row < target->row)
			hunter->row++;
		else if (hunter->row > target->row)
			hunter->row--;
	}
	if (logic == 0) {
		if (hunter->row < target->row)
			hunter->row++;
		else if (hunter->row > target->row)
			hunter->row--;
		if (hunter->col < target->col)
			hunter->col++;
		else if (hunter->col > target->col)
			hunter->col--;
	}
}
