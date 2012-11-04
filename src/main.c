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


int rows, cols; /* Current screen size globals  */
int level = 1;  /* Level of difficulty          */

/* Globals that may be set by command line arguments.
 * Initialized with default values. See enemies.c, player.c
 * and fobjects.c for details. */
double enemy_nokill_time        = ENEMY_NOKILL_TIME;
double enemy_row_delay          = ENEMY_ROW_DELAY_1;
double enemy_col_delay          = ENEMY_COL_DELAY_1;
int    fobj_init_delay_min      = FOBJ_INIT_DELAY_MIN;
int    fobj_init_delay_max      = FOBJ_INIT_DELAY_MAX;
int    fobj_hang_delay_min      = FOBJ_HANG_DELAY_MIN;
int    fobj_hang_delay_max      = FOBJ_HANG_DELAY_MAX;
double fobj_fall_delay_start    = FOBJ_FALL_DELAY_START;
double fobj_fall_delay_end      = FOBJ_FALL_DELAY_END;
double fobj_base                = FOBJ_BASE;
double fobj_acc                 = FOBJ_ACC;
double player_row_delay         = PLAYER_ROW_DELAY_1;
double player_col_delay         = PLAYER_COL_DELAY_1;
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
 * Set delays based on value of global variable level. setlevel() aborts
 * and exit(1) if argc != 1 or if level is something else than 1, 2 or 3.
 * It also sends an error message to stderr.
 */
void setlevel(int argc) {
	if (argc != 1 || level < 1 || level > 3) {
		fputs("Error: level is either 1, 2 or 3\n", stderr);
		exit(0);
	}
	/* If level is 1 delas are already set: this is the default */
	if (level == 2) {
		enemy_row_delay  = ENEMY_ROW_DELAY_2;
		enemy_col_delay  = ENEMY_COL_DELAY_2;
		player_row_delay = PLAYER_ROW_DELAY_2;
		player_col_delay = PLAYER_COL_DELAY_2;
	}
	else if (level == 3) {
		enemy_row_delay  = ENEMY_ROW_DELAY_3;
		enemy_col_delay  = ENEMY_COL_DELAY_3;
		player_row_delay = PLAYER_ROW_DELAY_3;
		player_col_delay = PLAYER_COL_DELAY_3;
	}
}

/*
 * Parse command line arguments
 */
static void parseargs(int argc, char *argv[])
{
	int c;
	/* One-char and words accepted as command line arguments */
	char *optstring = "f:hkl:sv";
	struct option longopts[] = {
		{"scorefile",   required_argument,  NULL, 'f'},
		{"help",        no_argument,        NULL, 'h'},
		{"vikeys",      no_argument,        NULL, 'k'},
		{"level",       required_argument,  NULL, 'l'},
		{"scores",      no_argument,        NULL, 's'},
		{"version",     no_argument,        NULL, 'v'},
		{0, 0, 0, 0}
	};
	char *fvalue = NULL;
	char *helpstring = "Usage: lwrace [-k] [-l <1-3>] [-f <file>]\n"
		               "       lwrace [-h | -v | -s]\n"
			"Run terminal mode game Lawyer Race, an enhanced clone of an old "
			"QBasic game.\nThe original plot was:\n"
			"\"Once mr O needed a lawyer for some reason, and so he hired one. "
			"When the\ncase was lost and all mr O:s money was gone with the "
			"wind, the lawyer\noffice still wanted its fee, and they set after "
			"mr O, who had fled to\nthe mountains, looking for dollars. Rocks "
			"are falling all around him while\nmr O is struggling to escape "
			"wild lawyers and to collect money enough to\nset him free. Play "
			"the game to find out if mr O will make it!\"\n"
			"Default game control: use arrow keys to move and space to stop.\n"
			"\n  -f  <file>, or     Use high scores file <file> instead\n"
			"  --scorefile <file> of ~/.lwrace.\n"
			"  -h, --help         Display this help and exit.\n"
			"  -k, --vikeys       Use hjkl instead of arrow keys.\n"
			"  -l <1-3>, or       Set difficulty of the game. 1 = easy "
								  "(default), 2 = medium\n"
			"  --level <1-3>      and 3 = hard. High scores are level "
								  "dependent.\n"
			"  -s, --scores       Display high score list and exit.\n"
			"  -v, --version      Output version information and exit.\n\n"
			"Report bugs to aa@mensa.se\n"
			"Github project page: <https://github.com/nissafors/lwrace>";

	/* Parse command line arguments and take action */
	while ((c = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) {
		switch (c) {
			int scanfargc;
			case 'h':
				puts(helpstring);
				exit(0);
			case 'f':
				fvalue = optarg;
				printf("f %s:\n", fvalue);
				puts("option f not implemented yet");
				break;
			case 'k':
				key_up    = VIK_UP;
				key_down  = VIK_DOWN;
				key_left  = VIK_LEFT;
				key_right = VIK_RIGHT;
				break;
			case 'l':
				/* Put numeric held by optarg in global var level */
				scanfargc = sscanf(optarg, "%d", &level);
				setlevel(scanfargc);
				break;
			case 's':
				puts("option s not implemented yet");
				break;
			case 'v':
				puts("Lawyer Race version 0.1");
				exit(0);
		}
	}
}

/*
 * Main
 */
int main(int argc, char *argv[])
{
	struct pos plpos;      /* Players position on screen */
	dir_t pldir;           /* Players direction */
	int score;             /* Players score */
	
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
	while(getch() != K_ENTER)
		;
	endwin();               /* Exit curses mode */
	return 0;
}
