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
#define HIT                        1
#define MISS                       0

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

/* Structs */
struct pos {
	int row;
	int col;
};

/* Typedefs */
typedef int dir_t;      /* Direction */
typedef int bool_t;     /* Boolean   */

/*** Prototypes ***/
bool_t      setpos(dir_t, struct pos *, double *, double *, double *);
struct pos  drawfigure (struct pos, char, struct pos, char, int, int);
double      getnow();
int         genrand(int, int);
