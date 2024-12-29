#include "chadware.h"
#include "chadgraphics.h"
#include <ncurses.h>
#define GAME_MEM 1024*256
#define N_PLAYERS 1
#define INFO_WIDTH 25
/* tomma chelbek, sigma */
WINDOW *info_win, *main_win;
int max_game_x, max_game_y, beg_game_x, beg_game_y;
int32_t n_events_input;
int32_t n_events_output;
char **players;
struct EventGeneric *events_in;
struct EntityGeneric *pj_generic;
struct EntityPlayer *pj;
struct EventGeneric *events_out;
int32_t pos_x, pos_y, hp;
void draw_game_content(int);

int main() {
	size_t ok_allocated;
	size_t i;
	char current_c;
	int game = 1, size_x, old_size_x, size_y, old_size_y, ticks_elapsed = 0;
	clock_t delta, oldtime = 0;
	double tps;
	// allocate events buffer memory. this buffer is the one used to communicate the game loop with the client/server
	// it is not the same as the one used internally by the game
	events_in = calloc(32, sizeof(struct EventGeneric));
	if(!events_in) {
		perror("Failed to allocate memory.");
		return 1;
	}
	events_out = calloc(32, sizeof(struct EventGeneric));
	if(!events_out) {
		perror("Failed to allocate memory.");
		return 1;
	}
	// allocate game memory and the strings (a 2d char array) for the players' names.
	ok_allocated = init_heap_chadware(GAME_MEM);
	if (!ok_allocated) {
		perror("Failed to allocate memory.");
		return 1;
	}
	players = calloc(1, sizeof(char*));
	if(!players) {
		perror("Failed to allocate memory.");
		return 1;
	}
	for (i = 0; i < N_PLAYERS; i++){
		players[i] = calloc(64, sizeof(char));
		if(!players[i]) {
			perror("Failed to allocate memory.");
			return 1;
		}
	}


	srand(clock() ^ ~(time(NULL)));
	seed = rand();

	strncpy(players[0], "carpatex", 64);
	if(init_chadware(1, players)) { // checks for errors during game init
		fputs("Unknown error initializating the game.\n", stderr);
	}
	pj_generic = &entityg_ptr[0];
	pj = entityg_ptr->data;
	pj_generic->location.galaxy = 1;
	pj_generic->location.planetary_system = 1;
	pj_generic->location.orbit = 4;
	pj_generic->location.surface = 0;
	pj_generic->pos.pos_x = 0;
	pj_generic->pos.pos_y = 0;
	initscr();
	raw();
	noecho();
	start_color();
	keypad(stdscr, TRUE);
	timeout(2);
	getmaxyx(stdscr, size_y, size_x);
	info_win = newwin(size_y, INFO_WIDTH, 0, size_x - INFO_WIDTH);
	main_win = newwin(size_y, size_x - INFO_WIDTH, 0, 0);
	init_char_colors();
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
		getmaxyx(main_win, max_game_y, max_game_x);
		getbegyx(main_win, beg_game_y, beg_game_x);

		draw_game_content(ticks_elapsed);
		box(main_win, 0, 0);
		wrefresh(main_win);

		box(info_win, 0, 0);
		// DRAW EVERYHTING
		mvwprintw(info_win, 1, 1, "SIZE_X: %d SIZE_Y: %d", getmaxx(stdscr), getmaxy(stdscr));
		mvwprintw(info_win, 2, 1, "TPS: %.2lf", tps);
		mvwprintw(info_win, 3, 1, "ELAPSED: %d", ticks_elapsed);
		mvwprintw(info_win, 4, 1, "KEY: %c", current_c);
		mvwprintw(info_win, 5, 1, "HP: %d", hp);
		mvwprintw(info_win, 7, 1, "POS X: %d", pj_generic->pos.pos_x);
		mvwprintw(info_win, 6, 1, "POS Y: %d", pj_generic->pos.pos_y);
		wrefresh(info_win);
		// DRAW EVERYTHING RELATED TO THE GAME'S WORLD HERE
		if (tick(n_events_input, events_in, &n_events_output, events_out) == -1) {
			goto gracefully_exit;
		}
		//print_grass(ticks_elapsed % 96, 1, 1, 10);
		//	reset_color_pairs();
		current_c = ' ';

		switch (getch()) {
			case 'w': case 'W': pj_generic->pos.pos_y++; break;
			case 's': case 'S': pj_generic->pos.pos_y--; break;
			case 'a': case 'A': pj_generic->pos.pos_x--; break;
			case 'd': case 'D': pj_generic->pos.pos_x++; break;
			case 'Q': case 'q': game = 0; break;
		}
		wclear(info_win);
		while(clock() < oldtime + NEXT_TICK);
		ticks_elapsed++;
	}
	wattr_set(main_win, A_NORMAL, 0, NULL);
gracefully_exit:
	endwin();
	free(events_in);
	free(events_out);
	return 0;
}

void draw_game_content(int current_tick) {
    int32_t i, j, k, chunk_index, tile_x, tile_y;
    int32_t max_chunk_x, max_chunk_y;
    int32_t start_chunk_x, start_chunk_y;

    // Calcular el número de chunks a mostrar en pantalla
    max_chunk_x = (int32_t)ceil((float)max_game_x / 16.0);
    max_chunk_y = (int32_t)ceil((float)max_game_y / 16.0);

    // Calcular el chunk de inicio basado en la posición del jugador
    start_chunk_x = (pj_generic->pos.pos_x / 16) - (max_chunk_x / 2);
    start_chunk_y = (pj_generic->pos.pos_y / 16) - (max_chunk_y / 2);

    k = 0;  // Reiniciar contador
    for (i = 0; i < max_chunk_y; i++) {
        for (j = 0; j < max_chunk_x; j++) {
            int chunk_x = start_chunk_x + j;
            int chunk_y = start_chunk_y + i;

            // Manejar coordenadas negativas correctamente
            lchunk_ptr[k].start_pos_x = chunk_x * 16;
            lchunk_ptr[k].start_pos_y = chunk_y * 16;
            lchunk_ptr[k].location = pj_generic->location;

            // Generar el chunk
            gen_chunk(seed, &lchunk_ptr[k]);

            k++;  // Incrementar índice del chunk
        }
    }

    // Dibujar los bloques con el eje Y invertido
    for (i = 0; i < max_game_y; i++) {
        for (j = 0; j < max_game_x; j++) {
            // Calcular las coordenadas globales con el eje Y invertido
            int global_x = start_chunk_x * 16 + j;
            int global_y = start_chunk_y * 16 + (max_game_y - i - 1); // Invertir el eje Y

            // Calcular el índice del chunk correspondiente
            int chunk_x_index = (global_x >= 0 ? global_x / 16 : (global_x - 15) / 16) - start_chunk_x;
            int chunk_y_index = (global_y >= 0 ? global_y / 16 : (global_y - 15) / 16) - start_chunk_y;

            chunk_index = chunk_x_index + chunk_y_index * max_chunk_x;

            // Verificar que el índice del chunk esté dentro del rango válido
            if (chunk_index < 0 || chunk_index >= max_chunk_x * max_chunk_y) {
                continue;
            }

            // Calcular las posiciones relativas del tile dentro del chunk
            tile_x = ((global_x % 16) + 16) % 16;
            tile_y = ((global_y % 16) + 16) % 16;

            // Verificar si el tile existe
            if (!lchunk_ptr[chunk_index].tile || !lchunk_ptr[chunk_index].tile[tile_y]) {
                continue;
            }

            // Dibujar el bloque con la posición vertical invertida
            p_natural_block(lchunk_ptr[chunk_index].tile[tile_y][tile_x], current_tick % 96, j, i);
        }
    }
}
