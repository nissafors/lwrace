#include <curses.h>
#include <string.h>
#include "lwrace.h"

/* Current screen size globals */
int Rows, Cols;

/* An ascii game using ncurses. Grab money while avoiding enemies and
 * falling objects. */
int main()
{
	struct pos plPos;       /* Players position on screen */
	int plDir;              /* Players direction */
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
	plDir = INIT; /* Don't move until a key is pressed. Can't use STOP because
                     player won't be drawn in that case. */
	getgamearea(Rows, Cols);        /* Get size of playground   */ 
	plPos.row = genrand(0, Rows);   /* Players initial position */
	plPos.col = genrand(0, Cols);
	score = 0;                      /* Reset score */

	/* Game loop */
	while ((plDir = getdir(plDir)) != EXIT) {
		getgamearea(Rows, Cols);        /* Get size of playground */ 
		plPos = drawPlayer(plDir, plPos);
		score += treasures(plPos);
		if (prows != Rows || pcols != Cols)
			for (pcols = 0; pcols < Cols; pcols++)
				mvaddch(prows, pcols, BACKGROUND);
		mvprintw(Rows, 0,"Score: %d", score);   /* Print score */
		prows = Rows;
		pcols = Cols;
		if(drawEnemies(plPos, score)) {
			mvaddch(plPos.row, plPos.col, DEAD);  /* Player killed */
			break;
		}
	}

	mvprintw(Rows/3,Cols/2-6,"GAME OVER!");
	mvprintw(Rows-Rows/3,Cols/2-6,"Press enter");
	refresh();
	nodelay(stdscr, FALSE);
	while(getch() != K_ENTER)
		;
	endwin();               /* Exit curses mode */
	return 0;
}
