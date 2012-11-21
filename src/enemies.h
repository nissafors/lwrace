/* Defines and prototypes for enemies.c, part of Lawyer Race
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

/* Defines */
#define MAX_ENEMIES                3
#define ADD_ENEMY_SCORE_INTERVAL   5
#define ENEMY_DELAY_DIFF           0

/* Enemy logic. The distance interval in which hunt logic other than default
 * will be used given in proportion of screen size (rows and columns). */
#define LOGIC_MIN_ROWQ            0.22
#define LOGIC_MIN_COLQ            0.0625
#define LOGIC_MAX_ROWQ            0.44
#define LOGIC_MAX_COLQ            0.375

/* Prototypes */
int hunt(struct pos *, struct pos *, int);
