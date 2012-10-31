#include <curses.h>
#include "globals.h"
#include "main.h"

/* Current screen size globals */
int rows, cols;

/* An ascii game using ncurses. Grab money while avoiding enemies and
 * falling objects. */
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
