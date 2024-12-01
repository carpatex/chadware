#include "chadware.h"
#include <ncurses.h>
#include <stdio.h>
#define GAME_MEM 1024*128
#define N_PLAYERS 1
#define INFO_WIDTH 25
/* tomma chelbek, sigma */
WINDOW *info_win, *main_win;
int max_game_x, max_game_y, beg_game_x, beg_game_y;
int32_t n_events_input;
int32_t n_events_output;
char **players;
struct EventGeneric *events_in;
struct EventGeneric *events_out;
int32_t pos_x, pos_y, hp;
void draw_game_content();
int main() {
	size_t i;
	char current_c;
	int game = 1, size_x, old_size_x, size_y, old_size_y, ticks_elapsed = 0;
	clock_t delta, oldtime = 0;
	double tps;
	// allocate events buffer memory. this buffer is the one used to communicate the game loop with the client/server
	// it is not the same as the one used internally by the game
	if(!(events_in = calloc(32, sizeof(struct EventGeneric)))) {
		perror("Failed to allocate memory.");
		return 1;
	}
	if(!(events_out = calloc(32, sizeof(struct EventGeneric)))) {
		perror("Failed to allocate memory.");
		return 1;
	}
	// allocate game memory and the strings (a 2d char array) for the players' names.
	if (!init_heap_chadware(GAME_MEM)) {
		perror("Failed to allocate memory.");
		return 1;
	}
	if(!(players = calloc(1, sizeof(char*)))) {
		perror("Failed to allocate memory.");
		return 1;
	}
	for (i = 0; i < N_PLAYERS; i++){
		if(!(players[i] = calloc(64, sizeof(char)))) {
			perror("Failed to allocate memory.");
			return 1;
		}
	}

	strncpy(players[0], "carpatex", 64);
	if(init_chadware(1, players)) { // checks for errors during game init
		fputs("Unknown error initializating the game.\n", stderr);
	}
	initscr();
	raw();
	noecho();
	start_color();
	keypad(stdscr, TRUE);
	timeout(2);
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
		mvwprintw(info_win, 4, 1, "KEY: %c", current_c);
		mvwprintw(info_win, 5, 1, "HP: %d", hp);
		mvwprintw(info_win, 6, 1, "POS X: %d", pos_x);
		wrefresh(info_win);
		// DRAW EVERYTHING RELATED TO THE GAME'S WORLD HERE
		if (tick(n_events_input, events_in, &n_events_output, events_out) == -1) {
			goto gracefully_exit;
		}
		getmaxyx(main_win, max_game_y, max_game_x);
		getbegyx(main_win, beg_game_y, beg_game_x);

		draw_game_content();
		//print_grass(ticks_elapsed % 96, 1, 1, 10);

		wrefresh(main_win);
		current_c = ' ';
		switch(getch()){
			case 'w':
			case 'W':
				current_c = 'w';
				break;
			case 'Q':
			case 'q':
				game = 0;
				break;
		}
		while(clock() < oldtime + NEXT_TICK);
		ticks_elapsed++;
	}
gracefully_exit:
	endwin();
	free(events_in);
	free(events_out);
	return 0;
}

void draw_game_content() {
	size_t i;
	struct EntityPlayer *pj  = entityg_ptr[0].data;
	--max_game_x;
	--max_game_y;
	++beg_game_x;
	++beg_game_y;
	// the limits of the screen have to have a 1 character margin on each side as otherwise it would replace the box.

}
