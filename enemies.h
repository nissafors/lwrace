/* Defines */
#define MAX_ENEMIES                3
#define ADD_ENEMY_SCORE_INTERVAL   5

/* Enemy logic. The distance interval in which hunt logic other than default
 * will be used given in proportion of screen size (rows and columns). */
#define LOGIC_MIN_ROWQ            0.22
#define LOGIC_MIN_COLQ            0.0625
#define LOGIC_MAX_ROWQ            0.44
#define LOGIC_MAX_COLQ            0.375

/* Timers - Time is given in seconds */
#define IMMORTAL_TIME             3 /* The number of seconds that player is
                                       untouchable when a new enemy is added */
#define ENEMY_DELAY_ROW           2.0125  /* from one side of the screen to  */
#define ENEMY_DELAY_COL           7.0     /* the other: rows and columns     */
#define ENEMY_DELAY_DIFF          0.0  /* This number is added to ENEMY_DELAY
                                          for every new enemy compared to last
                                          added enemy */

/* Prototypes */
dir_t hunt(struct pos *, struct pos *, int);
