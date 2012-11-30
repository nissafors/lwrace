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

#include <curses.h>     /* Must be included before main.h */
#include <string.h>
#include "globals.h"    /* Must be included before main.h */
#include "main.h"

/*
 * Set default values for global settings
 */
static void initglobals() {
	/* Default level of difficulty */
	level                   = 3;
	/* The time in seconds player is untouchable when a new enemy is added */
	enemy_nokill_time       = 3;       
	/* I think preferred ratio is 23/80, which will give the same delay for rows
	 * and cols on a standard terminal screen (which is 80*24, but the last row
	 * is reserved for score and info display in this game) */
	row_to_col_delay_ratio  = 23.0 / 80.0;
	/* See fobjects.c for an explanation of these */
	fobj_init_delay_min     = 24;
	fobj_init_delay_max     = 48;
	fobj_hang_delay_min     = 0;
	fobj_hang_delay_max     = 24;
	fobj_fall_delay_start   = 4.8;
	fobj_fall_delay_end     = 0.72;
	fobj_base               = 2;
	fobj_acc                = 16;
	/* Default keys for game control */
	key_esc                 = 27;
	key_pause               = 'p';
	key_stop                = ' ';
	key_enter               = '\n';
	key_up                  = KEY_UP;
	key_down                = KEY_DOWN;
	key_left                = KEY_LEFT;
	key_right               = KEY_RIGHT;
	key_file_path           = expandpath(DEFAULT_KEYFILE, 1);
	hiscore_file_path       = expandpath(DEFAULT_SCOREFILE, 1);
}

/*
 * Display a window with message + "Press enter" while waiting for keypress.
 * Before leaving delete the window and redraw stdscr.
 */
void idle(char *message) {
	int height, width, nwrow, nwcol;
	int message_len, presskey_msg_len;
	char *presskey_msg = "Press enter...";
	WINDOW *win;
	
	/* Set window size */
	message_len      = strlen(message);
	presskey_msg_len = strlen(presskey_msg);
	if (message_len >= presskey_msg_len) {
		width = message_len + 6;
	} else {
		width = presskey_msg_len + 6;
	}
	height = 7;
	nwrow = rows / 2.0 - height / 2.0;
	nwcol = cols / 2.0 - width / 2.0;
	
	/* Create and display window and messages */
	win = newwin(height,  width,  nwrow,  nwcol);
	box(win,  0, 0);
	mvwprintw(win, 2, width / 2.0 - strlen(message) / 2.0, message);
	mvwprintw(win, 4, width / 2.0 - strlen(presskey_msg) / 2.0, presskey_msg);
	wrefresh(win);
	
	/* Wait for keystroke */
	while(wgetch(win) != key_enter)
		;

	/* Kill windows, redraw stdscr and return to caller */
	delwin(win);
	redrawwin(stdscr);
	return;
}

/*
 * Main routine
 */
int main(int argc, char *argv[])
{
	struct pos plpos;      /* Players position on screen */
	dir_t pldir;           /* Players direction */
	dir_t lastdir;         /* Backup direction while pausing */
	int score;             /* Players score */
	char *name;            /* Players name */
	
	/* Set some globals */
	initglobals();
	/* Parse and implement command line arguments */
	parseargs(argc, argv);
	setspeed();
	if (setkeys(key_file_path) == 2) {
		return 1;          /* Quit on syntax error */
	}
	/* Seed random number generator */
	seedgenrand();

	/* ncurses settings */
	initscr();             /* Clear screen and enter curses mode */
	cbreak();              /* Line buffering disabled */
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
		/* Pause if user demands it */
		if (pldir == PAUSE) {
			idle("Game paused");
			pldir = lastdir;
		}
		lastdir = pldir;
		/* Get new screensize  */
		GETGAMEAREA(rows, cols);
		/* Update and print players position and score */
		plpos = drawplayer(pldir, plpos);
		score += treasures(plpos);
		printstatus(score);
		/* Update and draw enemies and falling objects. These return
		 * HIT (= TRUE) if player ran into any of them. */
		if(drawenemies(plpos, score) || fobjects(plpos, score)) {
			mvaddch(plpos.row, plpos.col, DEAD);  /* Player killed */
			idle("GAME OVER!");
			if (is_high_score(score, level, hiscore_file_path)) {
				/*** Store and print scores ***/
			}
			break;
		}
	}

	endwin();               /* Exit curses mode */
	return 0;
}
