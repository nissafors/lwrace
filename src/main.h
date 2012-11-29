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
#define GETGAMEAREA(R, C)  getmaxyx(stdscr, (R), (C)), rows--

/* * * * * *
 * Globals *
 * * * * * */
/* Current screen size globals */
int rows, cols;
/* Globals that may be set by command line arguments (now or in future versions
 * of this program). See enemies.c, player.c and fobjects.c for details. */
int    level;
double enemy_nokill_time;
double enemy_to_player_delay_ratio, row_to_col_delay_ratio;
double player_col_delay, player_row_delay, enemy_col_delay, enemy_row_delay;
int    fobj_init_delay_min, fobj_init_delay_max;
int    fobj_hang_delay_min, fobj_hang_delay_max;
double fobj_fall_delay_start, fobj_fall_delay_end;
double fobj_base, fobj_acc;
int    key_esc, key_pause, key_stop, key_enter;
int    key_up, key_down, key_left, key_right;
char  *key_file_path, *hiscore_file_path;

/* * * * * * * *
 * Prototypes  *
 * * * * * * * */
dir_t       getdir(dir_t);
struct pos  drawplayer(dir_t, struct pos);
void        printstatus(int);
bool_t      drawenemies(struct pos, int);
bool_t      fobjects(struct pos, int);
bool_t      treasures(struct pos);
void        parseargs(int argc, char *argv[]);
void        setspeed(void);
int         is_high_score(int score, int level, char *path);
char        *getname(void);
void        writescores(char* path, char *name, int score, int level);
void        seedgenrand(void);
int         setkeys(char* path);
