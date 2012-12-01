/* Part of Lawyer Race 
   Various helper functions
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

#include <stdlib.h>     /* For NULL */
#include <sys/time.h>
#include <stat.h>
#include <errno.h>

/*
 * Generate randon integer between, or equal to, min and max.
 * Do seedgenrand() before using.
 */
int genrand(int min, int max) {
	return rand() % (max-min) + min;
}

/*
 * Supply seed for genrand()
 */
void seedgenrand() {
	long seed;
	struct timeval now;
	gettimeofday(&now, NULL);
	
	seed = now.tv_sec * now.tv_usec;
	srand(seed);
}

/*
 * Return current time of day as secs.millisecs
 */
double getnow() {
	struct timeval now;

	gettimeofday(&now, NULL);
	return now.tv_sec+(now.tv_usec/1000000.0);
}

/*
 * Attempt to create default directory for storing high score files and
 * keyfile. Return values:
 * 0: Directory exists or was successfully created
 * 1: File exists but is not a directory
 * 2: Creating directory failed
 */
int mk_default_dir(char *path) {
	/*** Use stat() to check for existance.
	 * If found and is dir, return 0.
	 * If found and not dir, return 1.
	 * If not there, attempt to create using mkdir().
	 *   If successful return 0.
	 *   If we failed, return 2.
	 * TODO: Write prototype and function call to main.c.
}
