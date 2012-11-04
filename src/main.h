/* Defines and prototypes for main.c, part of Lawyer Race
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

/* * * * * * * * * * * * * * * * * * * * *
 * Command line arguments parsing stuff  *
 * * * * * * * * * * * * * * * * * * * * */
#include <getopt.h>
#include <stdlib.h>

/* * * * * *
 * Macros  *
 * * * * * */

/* Get screen size using curses function getmaxyx. Set aside one row as status
 * row for scores and messages. */
#define GETGAMEAREA(R, C)  getmaxyx(stdscr, (R), (C)), rows--;

/* * * * * * * * * * * * * * * * * * *
 * Default values for delay control  *
 * * * * * * * * * * * * * * * * * * */

/* Values postfixed by _1 is level 1 delays, as specified on command line.
 * values postfixed by _2 is level 2 delays and values postfixed by _3 is
 * level 3 delays
 */

/* falling objects */
#define FOBJ_INIT_DELAY_MIN    24
#define FOBJ_INIT_DELAY_MAX    48
#define FOBJ_HANG_DELAY_MIN     0
#define FOBJ_HANG_DELAY_MAX    24
#define FOBJ_FALL_DELAY_START   4.8
#define FOBJ_FALL_DELAY_END     0.72
#define FOBJ_BASE               2
#define FOBJ_ACC               16
/* enemies */
#define ENEMY_NOKILL_TIME       3      /* The number of seconds that player */
                                       /* is untouchable when a new enemy   */
                                       /* is added.                         */
#define ENEMY_ROW_DELAY_1       4.6    /* Travel time from one side of the  */
#define ENEMY_COL_DELAY_1      16.0    /* screen to the other.              */
#define ENEMY_ROW_DELAY_2       2.76
#define ENEMY_COL_DELAY_2       9.6
#define ENEMY_ROW_DELAY_3       2.0125
#define ENEMY_COL_DELAY_3       7.0
/* player */
#define PLAYER_ROW_DELAY_1      2.3    /* Travel time from one side of the  */
#define PLAYER_COL_DELAY_1      8.0    /* screen to the other.              */
#define PLAYER_ROW_DELAY_2      1.472
#define PLAYER_COL_DELAY_2      5.12
#define PLAYER_ROW_DELAY_3      1.15
#define PLAYER_COL_DELAY_3      4.0

/* * * * * * * * * * * * * *
 * Default values for keys *
 * * * * * * * * * * * * * */
#define K_ESC       27
#define K_PAUSE     'p'
#define K_STOP      ' '
#define K_ENTER     '\n'
#define K_UP        KEY_UP
#define K_DOWN      KEY_DOWN
#define K_LEFT      KEY_LEFT
#define K_RIGHT     KEY_RIGHT
#define VIK_UP      'k'
#define VIK_DOWN    'j'
#define VIK_LEFT    'h'
#define VIK_RIGHT   'l'

/* * * * * * * *
 * Prototypes  *
 * * * * * * * */
dir_t       getdir(dir_t);
struct pos  drawplayer(dir_t, struct pos);
void        printscore(int);
bool_t      drawenemies(struct pos, int);
bool_t      treasures(struct pos);
bool_t      fobjects(struct pos, int);
