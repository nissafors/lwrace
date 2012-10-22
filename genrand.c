#include <stdlib.h>
#include <sys/time.h>

/* Generate randon integer between, or equal to, min and max. Needs stdlib.h
 * and time.h */
int genrand(int min, int max) {
	struct timeval now;
	gettimeofday(&now, NULL);
	long seed = now.tv_sec * now.tv_usec;

	srand(seed);
	return rand() % (max-min) + min;
}
