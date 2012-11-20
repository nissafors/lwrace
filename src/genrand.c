/* Part of Lawyer Race 
   Generate randon integer between, or equal to, min and max.
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

int genrand(int min, int max) {
	static long lastseed;
	long seed;
	struct timeval now;
	gettimeofday(&now, NULL);
	
	seed = now.tv_sec * now.tv_usec;
	if (seed == lastseed) {
		seed++;
	}
	lastseed = seed;

	srand(seed);
	return rand() % (max-min) + min;
}
