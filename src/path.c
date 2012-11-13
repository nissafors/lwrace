/* Part of Lawyer Race 
   Expand relative paths
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

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

/*
 * #include <stdlib.h>
 * #include <sys/types.h>
 * #include <pwd.h>
 * Return a pointer to a string with homedir of user running this program (i.e. 
 * "/home/username").
 * Return NULL if uid not found or other getpwent() error occured.
 */
static char *getudir() {
	struct passwd *upwent;
	uid_t uid;

	uid = getuid();
	if ( (upwent = getpwent()) ) {
		/* Search for current uid in passwd file and return corresponding dir */
		while (upwent->pw_uid != uid) {
			if ( !(upwent = getpwent()) ) {
				/* Reached end of passwd file finding no matching uid */
				return NULL;
			}
		}
		return upwent->pw_dir;  /* SUCCESS! */ 
	}
	return NULL; /* First call to getpwent failed */
}

/*
 * #include <stdlib.h>
 * #include <string.h>
 * #include <unistd.h>
 * #include <limits.h> (for PATH_MAX if available)
 * Return a pointer to a string owned by malloc() with "path" expanded to an
 * absolute path. "path" may already be an absolute path, in which case it
 * is just copied to the heap to provide uniform behaviour: If "path" is
 * relative either to current dir or users homedir, or "path" is a simple file
 * name the full path will be expanded on the heap. Here are some valid examples
 * of "path"'s and what they may be expanded to:
 *   "/absolute/path/to/directory/" -> "/absolute/path/to/directory/"
 *   "~/relative/to/home/file.txt"  -> "/home/user/relative/to/home/file.txt"
 *   "./relative/to/cwd/"           -> "/current/directory/relative/to/cwd/"
 *   "file.txt"                     -> "/current/directory/file.txt"              
 * Returned pointer may be free()'d by caller. Return NULL on failure.
 * expandpath() does no checking if files and/or directories exists, but if
 * "isfile" is TRUE expandpath() returns NULL if last char in "path" is "/".
 */
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
char *expandpath(char *path, int isfile) {
	char firstchar, secondchar, lastchar;
	char cwd[PATH_MAX];
	char *dir = NULL;
	char *heap = NULL;
	int len = 0;

	firstchar = *path;
	secondchar = *(path + 1);
	lastchar = *(path + strlen(path) - 1);
	/* Freak out if isfile was TRUE and lastchar is / */
	if (lastchar == '/' && isfile)
		return NULL;
	/* Is path just a simple file name? */
	if (firstchar != '~' && firstchar != '/') {
		if ( !(firstchar == '.' && secondchar == '/') ) {
			if (!strchr(path, '/')) {
				/* Yes. Add to end of cwd and return pointer */
				if (getcwd(cwd, PATH_MAX)) {
					len = strlen(cwd) + strlen(path) + 2;
					if ( (heap = malloc(len * sizeof(char))) ) {
						sprintf(heap, "%s/%s", cwd, path);
						return heap;
					}
				}
			}

		}
	}
	/* Does path begin with '~/' or './'? */ 
	if ( (firstchar == '~' || firstchar == '.') && secondchar == '/') {
		/* Yes! Expand to absolute path on the heap and return pointer */
		if (firstchar == '~') {
			dir = getudir();
		} else {
			dir = getcwd(cwd, PATH_MAX);
		}
		if (dir) {
			if (strlen(dir) > PATH_MAX)
				return NULL; /* Path too long */
			/* Write dir + path to heap and return pointer. */
			len = strlen(dir) + strlen(path);  /* len + '\0' - '~' */
			if ( (heap = malloc(len * sizeof(char))) ) {
				sprintf(heap, "%s%s", dir, path + 1);
				return heap;
			}
		}
	} else if (firstchar == '/') {
		/* It's an absolute path: Just copy path to heap */
		heap = malloc((strlen(path) + 1) * sizeof(char));
		if (heap)
			if (strlen(path) <= PATH_MAX) /* Make sure path isn't too long */
				return strcpy(heap, path);
	}
	/* If we're here it was a bad formatted path, path is too long or
	 * getudir(), getcwd() or malloc() failed. */
	return NULL;
}
