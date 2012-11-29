#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <curses.h>

#define MAXLINE 128

int    key_up, key_down, key_left, key_right;
int    key_esc, key_pause, key_stop;

#define KEYWORDS_COUNT 7
struct keyword_global_pair {
	char *keyword;
	int *global;
} kw[] = {
	"up", &key_up,
	"down", &key_down,
	"left", &key_left,
	"right", &key_right,
	"stop", &key_stop,
	"pause", &key_pause,
	"exit", &key_esc
};

#define KEYS_COUNT 16
struct key_s {
	char *keyspec;
	int keynum;
} keys[] = {
	"<return>", '\n',
	"<tab>", 9,
	"<esc>", 27,
	"<space>", ' ',
	"<enter>", KEY_ENTER,
	"<down>", KEY_DOWN,
	"<up>", KEY_UP,
	"<left>", KEY_LEFT,
	"<right>", KEY_RIGHT,
	"<home>", KEY_HOME,
	"<backspace>", KEY_BACKSPACE,
	"<del>", KEY_DC,
	"<ins>", KEY_IC,
	"<pgdn>", KEY_NPAGE,
	"<pgup>", KEY_PPAGE,
	"<end>", KEY_END
};

/*
 * Search keys[] array for key specifiers and return corresponding number
 * if found. Return -1 on error.
 */
int lookup_key(char* keystr) {
	int i;
	for (i = 0; i < KEYS_COUNT; i++) {
		if (strcmp(keystr, keys[i].keyspec) == 0) {
			return keys[i].keynum;
		}
	}
	return -1;
}

/*
 * Examine value and assign proper key number to *key based on the result.
 * Return 0 on succes and 1 on error
 */
int parse_key(char *value, int *key) {
	int temp;

	/* Is it a printable char inside ''s? */
	if (value[0] == '\'' && value[2] == '\'' && strlen(value) == 3)  {
		*key = (int)value[1];
		return 0;
	}
	/* Is it a key specifier? */
	if ( (temp = lookup_key(value)) != -1 ) {
		*key = temp;
		return 0;
	}
	/* Is it a raw number? */
	if (sscanf(value, "%d", &temp) == 1) {
		*key = temp;
		return 0;
	}
	/* Nothing found, return error code */
	return 1;
}

/*
 * Read keyfile and look for lines starting with keywords "up, down, left,
 * right, stop, pause & exit". When found, send the rest of the string to
 * parse_key(). If invalid keyword found, or parse_key() returns true,
 * report syntax error to stderr and return 1 to caller. If no errors
 * found, return true.
 */
int setkeys(char* path) {
	FILE *file;
	char line[MAXLINE];
	char keyword[MAXLINE];
	char value[MAXLINE];
	int matches;
	int error_flag = 0;
	int i;

	/* Open file for reading */
	if ( ! (file = fopen(path, "r")) ) {
			return 1;
	}
	
	/* Read lines from file */
	while (fgets(line, MAXLINE, file) != NULL) {
		if (line[0] == '#') {
			continue;
		}
		
		/* Ket keyword and value from line */
		matches = sscanf(line, "%s%s", keyword, value);
		if (matches == EOF) {
			continue;
		}

		/* Compare word from line with valid keywords. On syntax error, report
		 * error and return 1 to caller. */
		error_flag = 1; /* Murphy's law */
		for (i = 0; i < KEYWORDS_COUNT; i++) {
			if (strcmp(keyword, kw[i].keyword) == 0) {
				error_flag = 0;
				if (parse_key(value, kw[i].global) != 0) {
					error_flag = 1;
				}
				break;
			}
		}
		if (error_flag) {
			error(0, 0, "Keyfile syntax error -- %s", path);
			return 1;
		}
	}
	/* Whole file read with no errors */
	return 0;
}

int main() {
	if(setkeys("keys")) {
		/* error */
		exit(0);
	}
	printf("up: %o down: %o left: %o right: %o stop: %o pause: %o esc: %o\n",
	       key_up, key_down, key_left, key_right, key_stop, key_pause, key_esc);
	return 0;
}
