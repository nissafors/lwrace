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

/* * * * * *
 * Macros  *
 * * * * * */

/* Get screen size using curses function getmaxyx. Set aside one row as status
 * row for scores and messages. */
#define GETGAMEAREA(R, C)  getmaxyx(stdscr, (R), (C)), rows--;

/* * * * * *
 * Globals *
 * * * * * */
int rows, cols; /* Current screen size globals          */
int level = 1;  /* Level of difficulty, init to default */

/* Globals that may be set by command line arguments (now or in future versions
 * of this program). Initialized with default values. See enemies.c, player.c
 * and fobjects.c for details. */

/* The number of seconds that player is untouchable when a new enemy is added */
double enemy_nokill_time     = 3;       
/* The time it takes for a player to travel from side to side of the screen */
double enemy_to_player_delay_ratio = 8.0 / 4.0;
double row_to_col_delay_ratio      = 23.0 / 80.0;
double player_col_delay            = 5.0;
/* Those are initialized by initglobals() */
double player_row_delay, enemy_col_delay, enemy_row_delay;
/* See fobjects for these */
int    fobj_init_delay_min   = 24;
int    fobj_init_delay_max   = 48;
int    fobj_hang_delay_min   = 0;
int    fobj_hang_delay_max   = 24;
double fobj_fall_delay_start = 4.8;
double fobj_fall_delay_end   = 0.72;
double fobj_base             = 2;
double fobj_acc              = 16;
/* Keys globals */
int    key_esc               = 27;
int    key_pause             = 'p';
int    key_stop              = ' ';
int    key_enter             = '\n';
int    key_up                = KEY_UP;
int    key_down              = KEY_DOWN;
int    key_left              = KEY_LEFT;
int    key_right             = KEY_RIGHT;

/* * * * * * * *
 * Prototypes  *
 * * * * * * * */
dir_t       getdir(dir_t);
struct pos  drawplayer(dir_t, struct pos);
void        printscore(int);
bool_t      drawenemies(struct pos, int);
bool_t      treasures(struct pos);
bool_t      fobjects(struct pos, int);
void        parseargs(int argc, char *argv[]);
