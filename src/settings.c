#include "globals.h"

/*
 * Set delays based on value of global variable level.
 * Level may be a number ranging from 0 to MAXLEVEL.
 */
void setlevel() {
	extern int level;
	extern double enemy_row_delay, enemy_col_delay;
	extern double player_row_delay, player_col_delay;
	extern double row_to_col_delay_ratio, enemy_to_player_delay_ratio;
	const double PCD_K  = 0.5,   PCD_M  = 1.5;
	const double ETPD_K = 0.125, ETPD_M = 1.125;

	player_col_delay = (MAXLEVEL + 1 - level) * PCD_K + PCD_M;
	enemy_to_player_delay_ratio = (MAXLEVEL + 1 - level) * ETPD_K + ETPD_M;
	player_row_delay = player_col_delay * row_to_col_delay_ratio;
	enemy_col_delay  = player_col_delay * enemy_to_player_delay_ratio;
	enemy_row_delay  = enemy_col_delay * row_to_col_delay_ratio;
}
