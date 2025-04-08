#ifndef _SETUP_H
#define _SETUP_H

#define CELLSTRMAX 13
#define BG_T 0
#define FG_T 1

void gol_map_setup(GOL_STATE*);
void state_opt_setcolor(char *, GOL_STATE *, uint8_t, uint8_t);
void gol_opt_parse(GOL_STATE *, int, char **);

void gol_setup(GOL_STATE*, int, char**);

#endif
