#include "lwrace.h"     /* For dir_t and struct pos */
#include <math.h>

/* Set pos based on dir if the time specified by *rd and *cd (row delay and
 * col delay) has passed since trec. Returns TRUE if delay time has elapsed
 * and FALSE otherwise */
bool_t setpos(dir_t dir, struct pos *rc, double *rd, double *cd, double *trec) {
	extern int rows, cols;
	const int TRUE = 1;
	const int FALSE = 0;
	double dd = sqrt(pow(*rd, 2) + pow(*cd, 2));

	switch (dir) {
		case UP:
			if (getnow() - *trec < *rd)
				return FALSE;
			rc->row--;
			break;
		case DOWN:
			if (getnow() - *trec < *rd)
				return FALSE;
			rc->row++;
			break;
		case RIGHT:
			if (getnow() - *trec < *cd)
				return FALSE;
			rc->col++;
			break;
		case LEFT:
			if (getnow() - *trec < *cd)
				return FALSE;
			rc->col--;
			break;
		case NE:
			if (getnow() - *trec < dd)
				return FALSE;
			rc->row--;
			rc->col++;
			break;
		case NW:
			if (getnow() - *trec < dd)
				return FALSE;
			rc->row--;
			rc->col--;
			break;
		case SE:
			if (getnow() - *trec < dd)
				return FALSE;
			rc->row++;
			rc->col++;
			break;
		case SW:
			if (getnow() - *trec < dd)
				return FALSE;
			rc->row++;
			rc->col--;
			break;
	}

	/* Check if new position is outside screen */
	if (rc->row < 0)
		rc->row = 0;
	if (rc->row >= rows - 1)
		rc->row = rows - 1;
	if (rc->col < 0)
		rc->col = 0;
	if (rc->col >= cols - 1)
		rc->col = cols - 1;

	/* Reset timer and return to caller */
	*trec = getnow();
	return TRUE;
}
