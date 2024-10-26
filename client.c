#include "chadware.h"
#include <ncurses.h>
#define INFO_WIDTH 25
/* tomma chelbek, sigma */

int main() {
	WINDOW *info_win, *main_win;
	int game = 1, size_x, old_size_x, size_y, old_size_y, ticks_elapsed = 0;
	clock_t delta, oldtime = 0;
	double tps;

	initscr();
	raw();
	noecho();
	start_color();
	keypad(stdscr, TRUE);
	timeout(1);
	getmaxyx(stdscr, size_y, size_x);
	info_win = newwin(size_y, INFO_WIDTH, 0, size_x - INFO_WIDTH);
	main_win = newwin(size_y, size_x - INFO_WIDTH, 0, 0);
	while(game) {
		delta = clock() - oldtime;
		tps = (1.0 / delta) * CLOCKS_PER_SEC;
		oldtime = clock();
		
		old_size_x = size_x;
		old_size_y = size_y;
		getmaxyx(stdscr, size_y, size_x);
		if(size_x != old_size_x || size_y != old_size_y) {
			resizeterm(size_y, size_x);
			info_win = newwin(size_y, INFO_WIDTH, 0, size_x - INFO_WIDTH);
			main_win = newwin(size_y, size_x - INFO_WIDTH, 0, 0);
		} 
		box(main_win, 0, 0);
		box(info_win, 0, 0);
		// DRAW EVERYHTING
		mvwprintw(main_win, 1, 1, "anashe");
		mvwprintw(info_win, 1, 1, "SIZE_X: %d SIZE_Y: %d", getmaxx(stdscr), getmaxy(stdscr));
		mvwprintw(info_win, 2, 1, "TPS: %.2lf", tps);
		mvwprintw(info_win, 3, 1, "ELAPSED: %d", ticks_elapsed);
		wrefresh(info_win);
		wrefresh(main_win);
		switch(getch()){
			case 'Q':
			case 'q':
				game = 0;
				break;
		}
		while(clock() < oldtime + NEXT_TICK);
		ticks_elapsed++;
	}
	endwin();
	return 0;
}
