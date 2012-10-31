/* Maximal number of objects at the same time */
#define MAX_OBJECTS                5
/* Score interval at which objects are added */
#define ADD_OBJECTS_SCORE_INTERVAL 5

/*
 * Delay control:
 * Before an object starts falling it will hang from the ceiling for a random
 * time between FOBJ_INIT_DELAY_MIN and FOBJ_INIT_DELAY_MAX. When it starts
 * falling the delay is defined by FOBJ_FALL_DELAY_START, and then accelerates
 * up to FOBJ_FALL_DELAY_END. The acceleration algorithm is a exponential
 * function where the base is FOBJ_BASE and "X" (the portion of the screen it
 * has fallen) is multiplied by FOBJ_ACC. When the objects has reached the
 * floor it returns to the ceiling with a new "hang" delay between
 * FOBJ_HANG_DELAY_MIN and FOBJ_HANG_DELAY_MAX. All delays will be divided
 * by rows to adjust for screen size.
 */
#define FOBJ_INIT_DELAY_MIN       24
#define FOBJ_INIT_DELAY_MAX       48
#define FOBJ_HANG_DELAY_MIN       0
#define FOBJ_HANG_DELAY_MAX       24
#define FOBJ_FALL_DELAY_START     4.8
#define FOBJ_FALL_DELAY_END       0.72
#define FOBJ_BASE                 2
#define FOBJ_ACC                 16
