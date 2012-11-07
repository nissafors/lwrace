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
	char *helpstring = "Usage: lwrace [-k] [-l <0-9>] [-f <file>]\n"
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
			"  -l <0-9>, or       Set difficulty of the game. 0-2 = easy"
								  ", 3 = default,\n"
			"  --level <0-9>      5-9 = medium to hard. High scores are level "
								  "dependent.\n"
			"  -s, --scores       Display high scores and exit.\n"
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
				if (scanfargc != 1 || level < 0 || level > MAXLEVEL) {
					fputs("Error: level out of range\n", stderr);
					exit(1);
				}
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
