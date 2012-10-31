/* Macros */
#define getgamearea(R, C)  getmaxyx(stdscr, (R), (C)), rows--;

/* Prototypes */
dir_t       getdir(dir_t);
struct pos  drawplayer(dir_t, struct pos);
void        printscore(int);
bool_t      drawenemies(struct pos, int);
bool_t      treasures(struct pos);
bool_t      fobjects(struct pos, int);
