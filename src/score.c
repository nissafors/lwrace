#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include "globals.h"

#define NAME_MAX 30
#define MAX_SCORES_PER_FILE 15

struct hiscore {
	char *name;
	int score;
};

/*
 * Show a dialog box and ask user for a name. Return a pointer to
 * a static array containing a string with the name. No error checking!
 */
char *getname() {
	extern int rows, cols;
	/* Dialog window size and position */
	const int dialog_height = 7;
	const int dialog_width  = NAME_MAX + 5;
	const int dialog_nwrow  = rows / 2.0 - dialog_height / 2.0;
	const int dialog_nwcol  = cols / 2.0 - dialog_width  / 2.0;
	/* Input window inside dialog window */
	const int input_height  = 3;
	const int input_width   = NAME_MAX + 3;
	const int input_nwrow   = dialog_nwrow + 3;
	const int input_nwcol   = dialog_nwcol + 1;
	/* Output and input */
	char* header = "HIGH SCORE!";
	char* text   = "Enter your name:";
	static char name[NAME_MAX + 1]; /* Name + \0 */
	WINDOW *dialog;
	WINDOW *input;

	/* Create and display windows */
	dialog = newwin(dialog_height, dialog_width, dialog_nwrow, dialog_nwcol);
	input  = newwin(input_height,  input_width,  input_nwrow,  input_nwcol);
	box(dialog, 0, 0);
	box(input,  0, 0);
	mvwprintw(dialog, 1, dialog_width / 2.0 - strlen(header) / 2.0, header);
	mvwprintw(dialog, 2, dialog_width / 2.0 - strlen(text  ) / 2.0, text  );
	wrefresh(dialog);
	wrefresh(input);
	
	/* Get user input */
	nodelay(input, FALSE);
	curs_set(2);
	echo();
	mvwgetnstr(input, 1, 1, name, NAME_MAX);

	/* Kill windows and return to caller */
	delwin(dialog);
	delwin(input);
	return name;
}

/*
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <error.h>
 * #include <errno.h>
 * #include <string.h>
 * Read at most MAX_SCORES_PER_FILE scores from scorefile with structure:
 * name\nscore\nname\nscore...
 * Return a pointer to a static array of struct hiscore's where every <var>.name
 * is a pointer to a string owned by malloc(). If file doesn't exist, return
 * NULL. On other errors: error(1, 0, msg).
 */
#define MAX_NUM 20 /* If someone gets a really good score on a 64 bit chip :) */
static struct hiscore *readscores (char *path) {
	static struct hiscore scores[MAX_SCORES_PER_FILE];
	char *heap;
	char numtemp[MAX_NUM]; 
	FILE *file;
	int eos, i;
	
	/* Open file for reading */
	if ( ! (file = fopen(path, "r")) ) {
		if (errno == ENOENT) {
			/* No such file or directory */
			return NULL;
		} else {
			error(1, 0, "unable to open high score file");
		}
	}
	
	/* Read at most MAX_SCORES_PER_FILE scores */
	for (i = 0; i < MAX_SCORES_PER_FILE; i++) {
		/* Read name, exit on error */
		if ( ! (heap = malloc(((NAME_MAX + 1) * sizeof(char)))) )
			error(1, 0, "memory problem when reading high score file");
		if ( ! (heap = fgets(heap, NAME_MAX, file)) ) {
			free(heap);
			break; /* Reached end of file */
		}
		/* Remove trailing \n and assign to array */
		eos = strlen(heap) - 1;
		if (heap[eos] == '\n')
			heap[eos] = '\0';
		scores[i].name = heap;
		/* Read score and assign to array, exit on error */
		if ( ! (fgets(numtemp, MAX_NUM, file)) )
			error(1, 0, "error reading line %d from high score file", i);
		if (sscanf(numtemp, "%d", &(scores[i].score)) != 1)
			error(1, 0, "expected number on line %d in high score file", i);
	}
	fclose(file);
	return scores;
}

/*
 * #include <stdio.h>
 * Draw a dashed horizontal line with <length> number of -'s. Terminate with
 * a newline.
 */
void dashes(length) {
	int i;

	for (i = 0; i <= length; i++) {
		printf("-");
	}
	printf("\n");
}

/*
 * #include <stdio.h>
 * #include <string.h>
 * Output at most MAX_SCORES_PER_FILE scores from array recieved from
 * readscores(path) in file "path" to stdout. If <path> does not end with
 * ".<number in range 0 to MAXLEVEL" print all existing files named "<path>.0"
 * to "<path>.<MAXLEVEL>".
 */
void printscores(char *path) {
	struct hiscore *scores;
	char *extpath = NULL;
	int printed_count = 0;
	char *heading = "  High Scores - level %d\n";
	char fromlevel, tolevel;
	int len, i;

	/* Is path *.<number>? */
	len = strlen(path);
	if ((path[len - 1] >= '0' && path[len - 1] <= '0' + MAXLEVEL) && 
	     path[len - 2] == '.')
	{
		/* Yes, this is a scorefile */
		fromlevel = tolevel = path[len - 1] - '0';
		/* Remove .<number> so we can treat all paths the same below */
		path[len - 2] = '\0';
	}
	else
	{
		/* Path is not a scorefile. Print all scorefiles in folder */
		fromlevel = 0;
		tolevel = MAXLEVEL;
	}

	len = strlen(path);
	extpath = malloc((len + 3) * sizeof(char));
	extpath = strcpy(extpath, path);
	for ( ; fromlevel <= tolevel; fromlevel++) {
		/* Add .<number> to path */
		extpath[len] = '.';
		extpath[len+1] = '0' + fromlevel;
		extpath[len+2] = '\0';
		/* Read scores to array and print a heading */
		if ( (scores = readscores(extpath)) ) {
			dashes(strlen(heading) + 2);
			printf(heading, fromlevel);
			dashes(strlen(heading) + 2);
		}
		/* Print array */
		for (i = 0; i < MAX_SCORES_PER_FILE; i++) {
			if (!scores)
				continue;
			if ((scores + i)->name) {
				printf("Score: %5d, by %s\n", (scores + i)->score,
						(scores + i)->name);
				printed_count++;
			}
			else {
				if(i == 0)
					error(1, 0, "file empty -- %s", extpath);
				else
					break;
			}
		}
	}
	if (printed_count == 0)
		error(1, 0, "unable to print high score file");
}
