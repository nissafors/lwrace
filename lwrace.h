/* Game control */
#define STOP                       0
#define UP                         1
#define DOWN                       2
#define LEFT                       3
#define RIGHT                      4
#define NE                         5
#define NW                         6
#define SE                         7
#define SW                         8
#define PAUSE                      9
#define INIT                      -1
#define EXIT                      -2
#define MAX_ENEMIES                3
#define MAX_OBJECTS                5
#define ADD_ENEMY_SCORE_INTERVAL   5
#define ADD_OBJECTS_SCORE_INTERVAL 5
#define HIT                        1
#define MISS                       0
#define PL_ADJ_MARGIN              5 /* Margin to corner if player were moved
                                       inside on screen size change. This is
                                       to avoid instant death if enemy also
                                       was moved to the same corner. */

/* Enemy logic. The distance interval in which hunt logic other than default
 * will be used given in proportion of screen size (rows and columns). */
#define LOGIC_MIN_ROWQ            0.22
#define LOGIC_MIN_COLQ            0.0625
#define LOGIC_MAX_ROWQ            0.44
#define LOGIC_MAX_COLQ            0.375

/* Timers - Time is given in seconds */
#define IMMORTAL_TIME             3 /* The number of seconds that player is
                                       untouchable when a new enemy is added */
#define PLAYER_DELAY_ROW          1.15    /* The time it should take for a   */
#define PLAYER_DELAY_COL          4.0     /* player, enemy or object to move */
#define ENEMY_DELAY_ROW           2.0125  /* from one side of the screen to  */
#define ENEMY_DELAY_COL           7.0     /* the other: rows and columns     */
#define ENEMY_DELAY_DIFF          0.0  /* This number is added to ENEMY_DELAY
                                          for every new enemy compared to last
                                          added enemy */
#define FOBJ_INIT_DELAY_MIN       24
#define FOBJ_INIT_DELAY_MAX       48
#define FOBJ_HANG_DELAY_MIN       0
#define FOBJ_HANG_DELAY_MAX       24
#define FOBJ_FALL_DELAY_START     4.8
#define FOBJ_FALL_DELAY_END       0.72
#define FOBJ_BASE                 2
#define FOBJ_ACC                 16

/* Screen size */
#define STDROWS  23 /* Normal number of legal rows for gameplay.
                       rows+1 = status bar */
#define STDCOLS  80 /* Normal number of columns for gameplay. */

/* Symbols for players, targets and enemies etc */
#define PLAYER      'O'
#define DEAD        'X'
#define ENEMY       '#'
#define TREASURE    '$'
#define OBJECT      'o'
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
#define getgamearea(R, C)  getmaxyx(stdscr, (R), (C)), rows--;

/* Structs */
struct pos {
	int row;
	int col;
};

/* Typedefs */
typedef int dir_t;      /* Direction */
typedef int bool_t;     /* Boolean   */

/*** Prototypes ***/
/* player.c */
dir_t getdir(dir_t);
struct pos getpos(struct pos, int);
struct pos drawplayer(dir_t, struct pos);
/* enemies.c */
bool_t drawenemies(struct pos, int);
dir_t hunt(struct pos *, struct pos *, int);
/* setpos.c */
bool_t setpos(dir_t, struct pos *, double *, double *, double *);
/* drawfigure.c */
struct pos drawfigure (struct pos, char, struct pos, char, int, int);
/* treasures.c */
bool_t treasures(struct pos);
/* getnow.c */
double getnow();
/* genrand.c */
int genrand(int, int);
/* fobjects.c */
bool_t fobjects(struct pos, int);
