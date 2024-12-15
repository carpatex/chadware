#ifndef CHADGRAPHICS_H
#define CHADGRAPHICS_H
#include "chadware.h"
#include <ncurses.h>
extern WINDOW *main_win, *info_win;
char i14toh(int32_t, int32_t); // converts integer from 0 to 15 starting from point to the respective hex character 
void p_natural_block(int32_t, int8_t, int, int);
void init_char_colors();
#endif
