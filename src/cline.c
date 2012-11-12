/*TODO
 * Option k
 * Option s
 * Option f
 * Option p
 */


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
#include <string.h>
#include "globals.h"
#include "cline.h"

/*
 * #include <string.h>
 * Display help texts. If optarg is not recognized as a keyword for specific
 * help, display usage info.
 */
static void showhelp(char *optarg) {
	if (optarg) {
		/* If we were called with "-h=keyword" we get "=keyword": remove '=' */
		if (*optarg == '=')
			optarg++;
		if (strcmp(optarg, "keyfile") == 0) {
			puts(keyfilehelp);
			return;
		} else if (strcmp(optarg, "scorefile") == 0) {
			puts(scorefilehelp);
			return;
		}
	}
	puts(usage);
}

/*
 * #include <stdio.h>
 * Set global var level to value held by string optarg. Set level
 * to -1 on error.
 */
static void setlevel(char *optarg) {
	extern int level;
	int scanfargc;

	scanfargc = sscanf(optarg, "%d", &level);
	if (scanfargc != 1 || level < 0 || level > MAXLEVEL) {
		level = -1; /* Error */
	}
}

/*
 * Set path to high score file. Valid formats is "~/path/to/file",
 * "./path/to/file", "/path/to/file" and "file". hiscrpath will
 * contain NULL if an error occured.
 */
static void sethiscrpath(char *optarg) {
	extern char *hiscore_file_path;
	hiscore_file_path = expandpath(optarg);
}

/*
 * Set path to keys file. Similar behaviour as sethiscrpath.
 */
static void setkeypath(char *optarg) {
	extern char *key_file_path;
	key_file_path = expandpath(optarg);
}

/*
 * #include <unistd.h>
 * #include <string.h>
 * Parse command line arguments
 */
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
#define OPTIONS_COUNT 7 /* Number of posts in longopts[] */
void parseargs(int argc, char *argv[])
{
	extern int level;
	int c, i;
	char argstring[OPTIONS_COUNT + 1];

	/* One-char and words accepted as command line arguments */
	char *optstring = "l:f:h::k:psv";
	/* If you add or remove options, change OPTIONS_COUNT accordingly */
	struct option longopts[] = {
		{"level",       required_argument,  NULL, 'l'},
		{"scorefile",   required_argument,  NULL, 'f'},
		{"help",        optional_argument,  NULL, 'h'},
		{"keyfile",     required_argument,  NULL, 'k'},
		{"plot",        no_argument,        NULL, 'p'},
		{"scores",      no_argument,        NULL, 's'},
		{"version",     no_argument,        NULL, 'v'},
		{0, 0, 0, 0}
	};

	/* Build a string containing all letters in optstring that was passed.
	 * If option has argument, take action so we don't have to save optarg. */
	i = 0;
	while ((c = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) {
		/* Invalid option or argument. getop_long() displays error msg for us */
		if (c == '?' || c == ':')
			exit(1);
		/* Halt if argument was passed twice. */
		if ( (strchr(argstring, c)) ) {
			fprintf(stderr, "%s: option may be given only once -- %c\n",
			        argv[0], c);
			exit(1);
		}
		/* JUMP TO CONCLUSIONS SECTION - Args that should be processed
		 * immediately when they show up */
		switch (c) {
			case 'l':
				setlevel(optarg);
				break;
			case 'f':
				sethiscrpath(optarg);
				break;
			case 'h':
				showhelp(optarg); /* Always be helpful... */
				exit(0);
			case 'k':
				setkeypath(optarg);
				break;
			default:
				/* Execute later: Save letter to end of string */
				argstring[i++] = (char)c;
		}
	}
	argstring[i] = '\0'; /* Terminate string */
	puts(argstring);
}
