/* An ascii game built on the curses library. Grab money while avoiding
   enemies and falling objects.
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
#include "main.h"

/* Current screen size globals */
int rows, cols;

/* Globals that may be set by command line arguments.
 * Initialized with default values. See enemies.c, player.c
 * and fobjects.c for details. */
double enemy_nokill_time        = ENEMY_NOKILL_TIME;
double enemy_row_delay          = ENEMY_ROW_DELAY;
double enemy_col_delay          = ENEMY_COL_DELAY;
int    fobj_init_delay_min      = FOBJ_INIT_DELAY_MIN;
int    fobj_init_delay_max      = FOBJ_INIT_DELAY_MAX;
int    fobj_hang_delay_min      = FOBJ_HANG_DELAY_MIN;
int    fobj_hang_delay_max      = FOBJ_HANG_DELAY_MAX;
double fobj_fall_delay_start    = FOBJ_FALL_DELAY_START;
double fobj_fall_delay_end      = FOBJ_FALL_DELAY_END;
double fobj_base                = FOBJ_BASE;
double fobj_acc                 = FOBJ_ACC;
double player_row_delay         = PLAYER_ROW_DELAY;
double player_col_delay         = PLAYER_COL_DELAY;
/* Keys globals */
int    key_esc                  = K_ESC;
int    key_pause                = K_PAUSE;
int    key_stop                 = K_STOP;
int    key_enter                = K_ENTER;
int    key_up                   = K_UP;
int    key_down                 = K_DOWN;
int    key_left                 = K_LEFT;
int    key_right                = K_RIGHT;

/* 
 */
int main()
{
	struct pos plpos;      /* Players position on screen */
	dir_t pldir;           /* Players direction */
	int score;             /* Players score */

	/* ncurses settings */
	initscr();             /* Clear screen and enter curses mode */
	raw();                 /* Line buffering disabled */
	keypad(stdscr, TRUE);  /* Enable F1-F12, arrow keys etc */
	noecho();              /* Supress unnecessary echoing */
	curs_set(0);           /* Hide the cursor */
	nodelay(stdscr, TRUE); /* Don't wait for keystrokes */

	/* init values for gameplay */
	pldir = INIT; /* Don't move until a key is pressed. Can't use STOP because
                     player won't be drawn in that case.        */
	getgamearea(rows, cols);        /* Get size of playground   */ 
	plpos.row = genrand(0, rows);   /* Randomize Players        */
	plpos.col = genrand(0, cols);   /* initial position.        */
	score = 0;                      /* Reset score              */

	/* Game loop */
	while ((pldir = getdir(pldir)) != EXIT) {
		/* Get new screensize  */
		getgamearea(rows, cols);
		/* Update and print players position and score */
		plpos = drawplayer(pldir, plpos);
		score += treasures(plpos);
		printscore(score);
		/* Update and draw enemies and falling objects. These return
		 * HIT (= TRUE) if player ran into any of them. */
		if(drawenemies(plpos, score) || fobjects(plpos, score)) {
			mvaddch(plpos.row, plpos.col, DEAD);  /* Player killed */
			break;
		}
	}

	/*** UGLY GAME OVER PLACEHOLDER!!! ***/
	mvprintw(rows/3,cols/2-6,"GAME OVER!");
	mvprintw(rows-rows/3,cols/2-6,"Press enter");
	refresh();
	nodelay(stdscr, FALSE);
	while(getch() != K_ENTER)
		;
	endwin();               /* Exit curses mode */
	return 0;
}
