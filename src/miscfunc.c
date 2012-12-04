/* Part of Lawyer Race 
   Various helper functions
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

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>

/*
 * Generate randon integer between, or equal to, min and max.
 * Do seedgenrand() before using.
 */
int genrand(int min, int max) {
    return rand() % (max-min) + min;
}

/*
 * Supply seed for genrand()
 */
void seedgenrand() {
    long seed;
    struct timeval now;
    gettimeofday(&now, NULL);
    
    seed = now.tv_sec * now.tv_usec;
    srand(seed);
}

/*
 * Return current time of day as secs.millisecs
 */
double getnow() {
    struct timeval now;

    gettimeofday(&now, NULL);
    return now.tv_sec+(now.tv_usec/1000000.0);
}

/*
 * Look for default directory and create it if it doesn't exist. If
 * path isn't a directory or if we can't create it, ask user if he
 * want's to play anyway. Exit if he doesn't. Must be called before entering
 * curses mode. Return values:
 * 0: path exists as a directory or was successfully created
 * 1: mkdir() failed or path exists but isn't a directory
 */
int mk_default_dir(char *path) {
    char *notdirerr = "%s exists but is not a directory.\n";
    char *othererr = "Error creating %s";
    char *play_anyway =
"Can't write high scores to default target\nfile or read default keyfile.\n"
"Play anyway? [y/n]";
    char reply;
    struct stat buf;
    int statret;
    int mkdirret;

    /* Get status for path */
    statret = stat(path, &buf);

    /* Check status for path and take action */
    if (statret == -1) {
        /* Create directory */
        mkdirret = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (mkdirret == 0) {
            /* Directory successfully created, return to caller */
            return 0;
        }
        /* mkdir() failed */
        error(0, 0, othererr, path);
    } else if (S_ISDIR(buf.st_mode)) {
        /* Directory exists, do nothing */
        return 0;
    } else {
        /* path exists but is not a directory. */
        error(0, 0, notdirerr, path);
    }
    /* Can't use default dir. Ask user if he want's to play anyway */
    while (1) {
        puts(play_anyway);
        reply = getchar();
        if (reply == 'n')
            exit(0);
        if (reply == 'y')
            break;
    }
    return 1;
}
