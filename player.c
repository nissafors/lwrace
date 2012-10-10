#include <curses.h>
#include "lwrace.h"

/* Read keybuffer and return direction, pause or exit. "curdir" is current
 * direction, which is returned if none of the specified keys have been
 * pressed */
int getdir(int curdir) {
  int key = getch();

  switch (key) {
    case K_UP:
      return UP;
    case K_DOWN:
      return DOWN;
    case K_LEFT:
      return LEFT;
    case K_RIGHT:
      return RIGHT;
    case K_STOP:
      return STOP;
    case K_PAUSE:
      return PAUSE;
    case K_ESC:
      return EXIT;
  }
  return curdir;  /* No valid keys pressed, return current direction */
}

/* Return new position based on direction dir and current position curPos */
struct pos getPos(struct pos curPos, int dir) {
  extern int Rows, Cols;

  switch (dir) {
    case UP:
      if (--curPos.row < 0)
        curPos.row = 0;
      break;
    case DOWN:
      if (++curPos.row >= Rows-1)
        curPos.row = Rows-1;
      break;
    case LEFT:
      if (--curPos.col < 0)
        curPos.col = 0;
      break;
    case RIGHT:
      if (++curPos.col >= Cols-1)
        curPos.col = Cols-1;
      break;
  }
  return curPos;
}

/* Draw player. "dir" is the direction in which player is moving. Return
 * players new position. */
struct pos drawPlayer(int dir, struct pos curpos) {
  extern int Rows, Cols;
  static double lasttime;
  struct pos newpos;

  if (getnow() - lasttime > PLAYER_DELAY) {
    newpos = getPos(curpos, dir);         /* Get new position based on dir */
    if (newpos.row > Rows)				  /* Player may be off screen if   */
	  newpos.row = Rows - PL_ADJ_MARGIN;  /* screen size has changed. Move */
    if (newpos.col > Cols)				  /* inside with a margin to avoid */
	  newpos.col = Cols - PL_ADJ_MARGIN;  /* instant death.                */
    if ((curpos.row == newpos.row && curpos.col == newpos.col 
        && dir != INIT) || dir == STOP)
      return curpos;    /* Don't redraw unless moved or need to initialize */
    mvaddch(curpos.row,curpos.col,BACKGROUND);        /* erase old player  */
    mvaddch(newpos.row,newpos.col,PLAYER);            /* draw new player   */
    curpos.row = newpos.row, curpos.col = newpos.col; /* remember position */
    lasttime = getnow();
  }
  return curpos;
}
