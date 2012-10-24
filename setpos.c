#include "lwrace.h"     /* For dir_t and struct pos */

/* Set pos based on dir if the time specified by delay has passed since
 * timerec. Returns TRUE if delay time has elapsed and FALSE otherwise */
int setpos(dir_t dir, struct pos *rc, double *delay, double *timerec) {
	extern int Rows, Cols;
	const int TRUE = 1;
	const int FALSE = 0;

	if (getnow() - *timerec > *delay) {
		switch (dir) {
			case UP:
				rc->row--;
				break;
			case DOWN:
				rc->row++;
				break;
			case RIGHT:
				rc->col++;
				break;
			case LEFT:
				rc->col--;
				break;
			case NE:
				rc->row--;
				rc->col++;
				break;
			case NW:
				rc->row--;
				rc->col--;
				break;
			case SE:
				rc->row++;
				rc->col++;
				break;
			case SW:
				rc->row++;
				rc->col--;
				break;
		}
		/* Check if new position is outside screen */
		if (rc->row < 0)
			rc->row = 0;
		if (rc->row >= Rows - 1)
			rc->row = Rows - 1;
		if (rc->col < 0)
			rc->col = 0;
		if (rc->col >= Cols - 1)
			rc->col = Cols - 1;
		return TRUE;
	}
	return FALSE;
}
