#include <curses.h>
#include "lwrace.h"

/* Current screen size globals */
int rows, cols;

/* An ascii game using ncurses. Grab money while avoiding enemies and
 * falling objects. */
int main()
{
	struct pos plpos;       /* Players position on screen */
	dir_t pldir;             /* Players direction */
	int score;              /* Players score */
	int prows, pcols;

	/* ncurses settings */
	initscr();                      /* Clear screen and enter curses mode */
	raw();                          /* Line buffering disabled */
	keypad(stdscr, TRUE);           /* Enable F1-F12, arrow keys etc */
	noecho();                       /* Supress unnecessary echoing */
	curs_set(0);                    /* Hide the cursor */
	nodelay(stdscr, TRUE);          /* Don't wait for keystrokes */

	/* init values for gameplay */
	pldir = INIT; /* Don't move until a key is pressed. Can't use STOP because
                     player won't be drawn in that case. */
	getgamearea(rows, cols);        /* Get size of playground   */ 
	plpos.row = genrand(0, rows);   /* Players initial position */
	plpos.col = genrand(0, cols);
	score = 0;                      /* Reset score */

	/* Game loop */
	while ((pldir = getdir(pldir)) != EXIT) {
		getgamearea(rows, cols);        /* Get new screensize  */
		plpos = drawplayer(pldir, plpos);
		score += treasures(plpos);
		if (prows != rows || pcols != cols)
			for (pcols = 0; pcols < cols; pcols++)
				mvaddch(prows, pcols, BACKGROUND);
		mvprintw(rows, 0,"Score: %d", score);   /* Print score */
		prows = rows;
		pcols = cols;
		if(drawenemies(plpos, score)) {
			mvaddch(plpos.row, plpos.col, DEAD);  /* Player killed */
			break;
		}
	}

	mvprintw(rows/3,cols/2-6,"GAME OVER!");
	mvprintw(rows-rows/3,cols/2-6,"Press enter");
	refresh();
	nodelay(stdscr, FALSE);
	while(getch() != K_ENTER)
		;
	endwin();               /* Exit curses mode */
	return 0;
}
