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
#include <error.h>
#include "globals.h"
#include "cline.h"

/*
 * #include <string.h>
 * Display help texts. If optarg is not recognized as a keyword for specific
 * help, display usage info.
 */
static void showhelp(char *optarg) {
	if (optarg) {
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
 * #include <error.h>
 * Check for illegal combinations of arguments. Return 0 if all is ok.
 * Illegal: p + any, v + any, s + any except f & l
 */
int incompatibleopts(char *argstring) {
	int errbool = 0;

	if ( (strchr(argstring, 'p') || strchr(argstring, 'v')) &&
	      strlen(argstring) > 1 )
	{
		errbool = 1;
	}
	if (strchr(argstring, 's')) {
		if (strlen(argstring) > 3)
			errbool = 1;
		if (strlen(argstring) == 3)
			if ( !(strchr(argstring, 'f') && strchr(argstring, 'l')) )
				errbool = 1;
		if (strlen(argstring) == 2)
			if ( !(strchr(argstring, 'f') || strchr(argstring, 'l')) )
				errbool = 1;
	}
	return errbool;
}

/*
 * #include <unistd.h>
 * #include <string.h>
 * Parse command line arguments. Depends on a few #defines i cline.h
 * Return a bitmask where LEVEL_ALTERED = 1, SCOREFILE_ALTERED = 2 and
 * KEYFILE_ALTERED = 4. These constants are defined in globals.h.
 */
int parseargs(int argc, char *argv[])
{
	extern int level;
	extern char *key_file_path;
	extern char *hiscore_file_path;
	int return_bits = 0;
	char *heap_area_for_scorefile_path;
	char *heap_area_for_keyfile_path;
	int c, i;
	char argstring[OPTIONS_COUNT + 1];
	argstring[0] = '\0';

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
		/* Remove = in front of optarg if present */
		if(optarg && *optarg == '=')
			optarg++;
		/* JUMP TO CONCLUSIONS SECTION - Args that should be processed
		 * immediately when they show up */
		switch (c) {
			case 'l':
				setlevel(optarg);
				return_bits = return_bits | LEVEL_ALTERED;
				break;
			case 'f':
				if( (heap_area_for_scorefile_path = expandpath(optarg, TRUE)) )
				{
					free(hiscore_file_path);
					hiscore_file_path = heap_area_for_scorefile_path;
					return_bits = return_bits | SCOREFILE_ALTERED;
				}
				break;
			case 'h':
				showhelp(optarg); /* Always be helpful... */
				exit(0);
			case 'k':
				if( (heap_area_for_keyfile_path = expandpath(optarg, TRUE)) )
				{
					free(key_file_path);
					key_file_path = heap_area_for_keyfile_path;
					return_bits = return_bits | KEYFILE_ALTERED;
				}
				break;
		}
		/* Save letter to end of string */
		argstring[i++] = (char)c;
		argstring[i] = '\0'; /* Terminate string */
	}
	
	/* Check for illegal combination of options */
	if (incompatibleopts(argstring))
		error(1, 0, "illegal combination of arguments -- %s", argstring);
	/* Did any option with argument fail? */
	if (level == -1) {
		fprintf(stderr, "%s: level out of range\n", argv[0]);
		exit(1);
	}
	if ( (strchr(argstring, 'f') && !hiscore_file_path) ||
	     (strchr(argstring, 'k') && !key_file_path) )
	{
		fprintf(stderr, "%s: could not expand path\n", argv[0]);
		exit(1);
	}

	/* Execute remaining commands */
	if (strchr(argstring, 's')) {
		if (!strchr(argstring, 'l')) {
			/* no level argument: tell printscores() to print all levels by
			 * sending negative level */
			level = -1;
		}
		printscores(hiscore_file_path, level);
		exit(0);
	}
	if (*argstring == 'p') {
		puts(plot);
		exit(0);
	}
	if (*argstring == 'v') {
		printf(version, LWRACE_VERSION);
		exit(0);
	}
	return return_bits;
}
