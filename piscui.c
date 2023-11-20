#include <ncurses.h>
#define INFO_WIDTH 22

int main () {
	WINDOW *info_win, *main_win;
	int size_x, size_y;

	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	refresh();

	getmaxyx(stdscr, size_y, size_x);
	info_win = newwin(size_y, INFO_WIDTH, 0, size_x - INFO_WIDTH);
	main_win = newwin(size_y, size_x - INFO_WIDTH, 0, 0);
	box(main_win, 0, 0);
	box(info_win, 0, 0);
	mvwprintw(main_win, 1, 1, "anashe");
	mvwprintw(info_win, 1, 1, "%d %d", getmaxx(stdscr), getmaxy(stdscr));
	wrefresh(main_win);
	wrefresh(info_win);

	getch();
	endwin();
	return 0;
}

