/* Part of Lawyer Race 
   Functions to handle the player
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
#include <string.h>
#include "global.h"

/*
 * Read keybuffer and return direction, pause or exit. "curdir" is current
 * direction, which is returned if none of the specified keys have been
 * pressed
 */
dir_t getdir(dir_t curdir) {
    extern int key_up, key_down, key_left, key_right;
    extern int key_stop, key_pause, key_esc;

    int key = getch();

    if (key == key_up)
        return UP;
    if (key == key_down)
        return DOWN;
    if (key == key_left)
        return LEFT;
    if (key == key_right)
        return RIGHT;
    if (key == key_stop)
        return STOP;
    if (key == key_pause)
        return PAUSE;
    if (key == key_esc)
        return EXIT;
    return curdir;  /* No valid keys pressed, return current direction */
}

/*
 * Draw player. "dir" is the direction in which player is moving. Return
 * players new position. Delay control is in header.
 */
struct pos drawplayer(dir_t dir, struct pos curpos) {
    extern int    rows, cols;
    extern double player_row_delay;
    extern double player_col_delay;
    static double lasttime;           /* Used by setpos() for delay timer */
    static int    lastrows, lastcols; /* used by drawfigure() */
    struct pos    oldpos;
    double        rdelay = player_row_delay / rows;
    double        cdelay = player_col_delay / cols;

    /* Set new position and draw player */
    oldpos = curpos;
    if (setpos(dir, &curpos, &rdelay, &cdelay, &lasttime) || dir == INIT) {
        curpos = drawfigure(curpos, PLAYER, oldpos, BACKGROUND,
                            lastrows, lastcols);
        lastrows = rows, lastcols = cols; /* Remember screen size */
    }
    /* Go home */
    return curpos;
}

/*
 * #include <curses.h>
 * #include <string.h>
 * Print score and level at last row. Erase old status row first if
 * screen size changed
 */
void printstatus (int score) {
    extern int rows;
    extern int cols;
    extern int level;
    static int oldscrrow;
    int        clrcol;
    char      *scoref = "Score: %d";
    char      *levelf = "Level: %d";
    
    /* Erase old status row if screensize changed */
    if (oldscrrow != rows)
        for (clrcol = 0; clrcol < cols; clrcol++)
            mvaddch(oldscrrow, clrcol, BACKGROUND);
    /* Print score and level */
    mvprintw(rows, 0, scoref, score);
    mvprintw(rows, cols - strlen(levelf), levelf, level);
    /* Save current status row position */
    oldscrrow = rows;
}
