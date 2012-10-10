#include <curses.h>
#include "lwrace.h"

/* Add a treasure to screen that player should try to get. Return 1 if he
 * catches one, return 0 if not. */
int treasures(struct pos plPos) {
  extern int Rows, Cols;
  static struct pos trPos;
  static int trCount;

  if (!trCount) {                 /* New treasure needed */
    trPos.row = genrand(0, Rows);
    trPos.col = genrand(0, Cols);
    trCount++;
  }
  
  if (trPos.row > Rows) trPos.row = Rows - 1; /* If screen size has changed  */
  if (trPos.col > Cols) trPos.col = Cols - 1; /* treasure may be unreachable */

  mvaddch(trPos.row, trPos.col, TREASURE);  /* Draw outside if's, in case
											   enemy erases it */
  if (plPos.row == trPos.row && plPos.col == trPos.col) { /* Player hit it */
    trCount--;
    mvaddch(trPos.row, trPos.col, PLAYER);  /* Use player char to erase old
                                               treasure, to cover the situation
                                               where player stopped on top of
                                               treasure */
    return 1;
  }
  return 0;
}
