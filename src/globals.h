/* Defines and prototypes used by several Lawyer Race files
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

/* Game control */
#define STOP                       0
#define UP                         1
#define DOWN                       2
#define LEFT                       3
#define RIGHT                      4
#define NE                         5
#define NW                         6
#define SE                         7
#define SW                         8
#define PAUSE                      9
#define INIT                      -1
#define EXIT                      -2
#define HIT                        1
#define MISS                       0
#define MAXLEVEL                   9    /* Remember to change helpstring if
                                           MAXLEVEL is altered */

/* Symbols for players, targets and enemies etc */
#define PLAYER      'O'
#define DEAD        'X'
#define ENEMY       '#'
#define TREASURE    '$'
#define OBJECT      'o'
#define BACKGROUND  ' '

/* Structs */
struct pos {
	int row;
	int col;
};

/* Typedefs */
typedef int dir_t;      /* Direction */
typedef int bool_t;     /* Boolean   */

/*** Prototypes ***/
bool_t      setpos(dir_t, struct pos *, double *, double *, double *);
struct pos  drawfigure (struct pos, char, struct pos, char, int, int);
double      getnow();
int         genrand(int, int);
