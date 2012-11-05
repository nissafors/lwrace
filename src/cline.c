/* Part of Lawyer Race 
   Functions for command line arguments parsing and implementation
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

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "globals.h"

#define VIK_UP      'k'
#define VIK_DOWN    'j'
#define VIK_LEFT    'h'
#define VIK_RIGHT   'l'

/*
 * Set delays based on value of global variable level. setlevel() aborts
 * and exit(1) if argc != 1 or if level is something else than 1, 2 or 3.
 * It also sends an error message to stderr.
 */
static void setlevel(int argc) {
	extern int level;
	extern double enemy_row_delay, enemy_col_delay;
	extern double player_row_delay, player_col_delay;
	extern double row_to_col_delay_ratio, enemy_to_player_delay_ratio;

	if (argc != 1 || level < 1 || level > 3) {
		fputs("Error: level is either 1, 2 or 3\n", stderr);
		exit(0);
	}
	/* If level is 1 delays are already set: this is the default */
	if (level > 1 || level < 4) {
		if (level == 2) {
			player_col_delay = player_col_delay * 0.8;
			enemy_to_player_delay_ratio = 7.0/4.0;
		}
		else if (level == 3) {
			player_col_delay = player_col_delay * 0.6;
			enemy_to_player_delay_ratio = 6.0/4.0;
		}
		player_row_delay = player_col_delay * row_to_col_delay_ratio;
		enemy_col_delay  = player_col_delay * enemy_to_player_delay_ratio;
		enemy_row_delay  = enemy_col_delay * row_to_col_delay_ratio;
	}
}

/* 
 * Set game control keys to something else than default (currently vi keys)
 */
void setkeys() {
	extern int key_up, key_down, key_left, key_right;

	key_up    = VIK_UP;
	key_down  = VIK_DOWN;
	key_left  = VIK_LEFT;
	key_right = VIK_RIGHT;
}

/*
 * Parse command line arguments
 */
void parseargs(int argc, char *argv[])
{
	extern int level;
	int c;
	/* One-char and words accepted as command line arguments */
	char *optstring = "f:hkl:sv";
	struct option longopts[] = {
		{"scorefile",   required_argument,  NULL, 'f'},
		{"help",        no_argument,        NULL, 'h'},
		{"vikeys",      no_argument,        NULL, 'k'},
		{"level",       required_argument,  NULL, 'l'},
		{"scores",      no_argument,        NULL, 's'},
		{"version",     no_argument,        NULL, 'v'},
		{0, 0, 0, 0}
	};
	char *fvalue = NULL;
	char *helpstring = "Usage: lwrace [-k] [-l <1-3>] [-f <file>]\n"
		               "       lwrace [-h | -v | -s]\n"
			"Run terminal mode game Lawyer Race, an enhanced clone of an old "
			"QBasic game.\nThe original plot was:\n"
			"\"Once mr O needed a lawyer for some reason, and so he hired one. "
			"When the\ncase was lost and all mr O:s money was gone with the "
			"wind, the lawyer\noffice still wanted its fee, and they set after "
			"mr O, who had fled to\nthe mountains, looking for dollars. Rocks "
			"are falling all around him while\nmr O is struggling to escape "
			"wild lawyers and to collect money enough to\nset him free. Play "
			"the game to find out if mr O will make it!\"\n"
			"Default game control: use arrow keys to move and space to stop.\n"
			"\n  -f  <file>, or     Use high scores file <file> instead\n"
			"  --scorefile <file> of ~/.lwrace.\n"
			"  -h, --help         Display this help and exit.\n"
			"  -k, --vikeys       Use hjkl instead of arrow keys.\n"
			"  -l <1-3>, or       Set difficulty of the game. 1 = easy "
								  "(default), 2 = medium\n"
			"  --level <1-3>      and 3 = hard. High scores are level "
								  "dependent.\n"
			"  -s, --scores       Display high score list and exit.\n"
			"  -v, --version      Output version information and exit.\n\n"
			"Report bugs to aa@mensa.se\n"
			"Github project page: <https://github.com/nissafors/lwrace>";

	/* Parse command line arguments and take action */
	while ((c = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) {
		switch (c) {
			int scanfargc;
			case 'h':
				puts(helpstring);
				exit(0);
			case 'f':
				fvalue = optarg;
				printf("f %s:\n", fvalue);
				puts("option f not implemented yet");
				break;
			case 'k':
				setkeys();
				break;
			case 'l':
				/* Put numeric held by optarg in global var level */
				scanfargc = sscanf(optarg, "%d", &level);
				setlevel(scanfargc);
				break;
			case 's':
				puts("option s not implemented yet");
				break;
			case 'v':
				puts("Lawyer Race version 0.1");
				exit(0);
		}
	}
}
