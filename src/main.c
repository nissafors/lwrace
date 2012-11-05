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

/*
 * Set defaults for globals with calculated init values.
 */
void initglobals() {
	player_row_delay = player_col_delay * row_to_col_delay_ratio;
	enemy_col_delay = player_col_delay * enemy_to_player_delay_ratio;
	enemy_row_delay = enemy_col_delay * row_to_col_delay_ratio;
}

/*
 * Main routine
 */
int main(int argc, char *argv[])
{
	struct pos plpos;      /* Players position on screen */
	dir_t pldir;           /* Players direction */
	int score;             /* Players score */
	
	/* Set some globals */
	initglobals();
	/* Parse and implement command line arguments */
	parseargs(argc, argv);

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
	GETGAMEAREA(rows, cols);        /* Get size of playground   */ 
	plpos.row = genrand(0, rows);   /* Randomize Players        */
	plpos.col = genrand(0, cols);   /* initial position.        */
	score = 0;                      /* Reset score              */

	/* Game loop */
	while ((pldir = getdir(pldir)) != EXIT) {
		/* Get new screensize  */
		GETGAMEAREA(rows, cols);
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
	while(getch() != key_enter)
		;
	endwin();               /* Exit curses mode */
	return 0;
}
