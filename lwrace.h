/* Game control */
#define STOP                      0
#define UP                        1
#define DOWN                      2
#define LEFT                      3
#define RIGHT                     4
#define PAUSE                     5
#define INIT                      6
#define EXIT                     -1
#define MAX_ENEMIES               3
#define ADD_ENEMY_SCORE_INTERVAL  5
#define HIT                       1
#define MISS                      0
#define PL_ADJ_MARGIN             5 /* Margin to corner if player were moved
									   inside on screen size change. This is
									   to avoid instant death if enemy also
									   was moved to the same corner. */

/* Timers - Time is given in seconds */
#define IMMORTAL_TIME             3
#define PLAYER_DELAY              0.05 /* This is the delays when screen size */
#define ENEMY_DELAY               0.1  /* is STDROWS * STDCOLS. It's auto-    */
#define OBJECT_DELAY              0.05 /* adjusted if screen size is altered. */
#define ENEMY_DELAY_DIFF          0.05 /* DIFF is added to ENEMY_DELAY for
                                          every new enemy compared to last
                                          added enemy */

/* Screen size */
#define STDROWS  23 /* Normal number of legal rows for gameplay.
					   Rows+1 = status bar */
#define STDCOLS  80	/* Normal number of columns for gameplay. */

/* Symbols for players, targets and enemies etc */
#define PLAYER      'O'
#define DEAD        'X'
#define ENEMY       '#'
#define TREASURE    '$'
#define BACKGROUND  ' '

/* Keys */
#define K_ESC   27
#define K_PAUSE 'p'
#define K_STOP  ' '
#define K_ENTER '\n'
#define K_UP    KEY_UP
#define K_DOWN  KEY_DOWN
#define K_LEFT  KEY_LEFT
#define K_RIGHT KEY_RIGHT
/*#define K_UP    'k'
#define K_DOWN  'j'
#define K_LEFT  'h'
#define K_RIGHT 'l'*/

/* Macros */
#define getgamearea(R, C)  getmaxyx(stdscr, (R), (C)), Rows--;

/* Structs */
struct pos {
  int row;
  int col;
};

/*** Prototypes ***/
/* player.c */
int getdir();
struct pos getPos(struct pos, int);
struct pos drawPlayer(int, struct pos);
/* enemies.c */
int drawEnemies(struct pos, int);
void hunt(struct pos *, struct pos *, int);
/* treasures.c */
int treasures(struct pos);
/* getnow.c */
double getnow();
/* genrand.c */
int genrand(int, int);
