
#include "chadware.h"
#include "chadgraphics.h"
#include <ncurses.h>
#include <unistd.h>
#include <math.h>

#define GAME_MEM 1024*256
#define N_PLAYERS 1
#define INFO_WIDTH 25

// Global variables
WINDOW *info_win, *main_win;
int max_game_x, max_game_y, beg_game_x, beg_game_y;
int32_t n_events_input;
int32_t n_events_output;
char **players;
struct EventGeneric *events_in;
struct EntityGeneric *pj_generic;
struct EntityPlayer *pj;
struct EventGeneric *events_out;
struct LoadedChunk *client_chunks;
int32_t pos_x, pos_y, hp;

// Function declarations
void initialize_memory();
void initialize_game();
void setup_ncurses();
void cleanup();
void handle_resize();
void draw_ui(int ticks_elapsed, double tps, char current_c);
void handle_input(int *game);
void draw_game_content(int current_tick);

int main() {

	size_t ticks_elapsed = 0;
	char current_c = ' ';
	int game = 1, size_x, size_y;
	clock_t oldtime = clock(), delta;
	double tps;

	// Initialization
	initialize_memory();
	initialize_game();
	setup_ncurses();

	// Main game loop
	while (game) {
		delta = clock() - oldtime;
		tps = (1.0 / delta) * CLOCKS_PER_SEC;
		oldtime = clock();
		struct GenChunkEvent chunk_event = {
			.entity_index = 0,
			.target_size = 256,
			.target = client_chunks
		};

		// Queue the chunk event
		events_in[0].event_id = 3; // Event ID for chunk generation
		events_in[0].data = &chunk_event;
		n_events_input = 1;

		handle_resize();
		tick(n_events_input, events_in, &n_events_output, events_out);

		wclear(main_win);
		draw_game_content(ticks_elapsed);
		wrefresh(main_win);  // Ensure main window is updated

		draw_ui(ticks_elapsed, tps, current_c);

		handle_input(&game);

		ticks_elapsed++;

		// Wait until the next tick
		while (clock() < oldtime + NEXT_TICK);
	}

	// Cleanup
	cleanup();
	return 0;
}

void initialize_memory() {
	client_chunks = calloc(256, sizeof(struct LoadedChunk));
	if (!client_chunks) {
		perror("Failed to allocate memory for client_chunks.");
		exit(1);
	}
	events_in = calloc(32, sizeof(struct EventGeneric));
	events_out = calloc(32, sizeof(struct EventGeneric));
	if (!events_in || !events_out) {
		perror("Failed to allocate memory for events.");
		exit(1);
	}

	if (!init_heap_chadware(GAME_MEM)) {
		perror("Failed to allocate game memory.");
		exit(1);
	}

	players = calloc(N_PLAYERS, sizeof(char*));
	if (!players) {
		perror("Failed to allocate memory for players.");
		exit(1);
	}

	for (size_t i = 0; i < N_PLAYERS; i++) {
		players[i] = calloc(64, sizeof(char));
		if (!players[i]) {
			perror("Failed to allocate memory for player name.");
			exit(1);
		}
	}
}

void initialize_game() {
	srand(clock() ^ ~(time(NULL)));
	strncpy(players[0], "carpatex", 64);

	if (init_chadware(1, players)) {
		fputs("Unknown error initializing the game.", stderr);
		exit(1);
	}

	pj_generic = &entityg_ptr[0];
	pj = entityg_ptr->data;
	pj_generic->location.galaxy = 1;
	pj_generic->location.planetary_system = 1;
	pj_generic->location.orbit = 4;
	pj_generic->location.surface = 0;
	pj_generic->pos.pos_x = 0;
	pj_generic->pos.pos_y = 0;
}

void setup_ncurses() {
	initscr();
	raw();
	noecho();
	start_color();
	keypad(stdscr, TRUE);
	timeout(2);

	int size_x, size_y;
	getmaxyx(stdscr, size_y, size_x);
	info_win = newwin(size_y, INFO_WIDTH, 0, size_x - INFO_WIDTH);
	main_win = newwin(size_y, size_x - INFO_WIDTH, 0, 0);

	init_char_colors();
}


void handle_resize() {
	static int old_size_x = 0, old_size_y = 0;

	// Obtener las dimensiones actuales de la pantalla
	getmaxyx(stdscr, max_game_y, max_game_x); // max_game_x y max_game_y son variables globales
	getbegyx(stdscr, beg_game_y, beg_game_x);  // Estas variables también se actualizan

	// Si las dimensiones de la pantalla cambiaron, ajustamos las ventanas
	if (max_game_x != old_size_x || max_game_y != old_size_y) {
		resizeterm(max_game_y, max_game_x); // Redimensionar la terminal según los nuevos tamaños
		info_win = newwin(max_game_y, INFO_WIDTH, 0, max_game_x - INFO_WIDTH); // Actualizar ventana de información
		main_win = newwin(max_game_y, max_game_x - INFO_WIDTH, 0, 0);            // Actualizar ventana principal
		old_size_x = max_game_x;
		old_size_y = max_game_y;
	}

	// Acceder al jugador a través de entityg_ptr y su índice (suponemos que el jugador es el índice 0)
	struct EntityGeneric *player_entity = &entityg_ptr[0]; // Suponiendo que el índice 0 es el jugador
	if (!player_entity || !player_entity->data) {
		fprintf(stderr, "Jugador no encontrado o datos inválidos.\n");
		return;
	}

	// Obtener datos específicos del jugador
	struct EntityPlayer *player = (struct EntityPlayer *)player_entity->data;

	// Obtener las coordenadas de la posición del jugador
	int32_t player_pos_x = player_entity->pos.pos_x;  // Usar pos_x de EntityPositionSurface
	int32_t player_pos_y = player_entity->pos.pos_y;  // Usar pos_y de EntityPositionSurface

	// Ahora ajustamos los límites de los chunks con base en la posición central del jugador
	// Establecemos los límites del jugador (ep_limit_nw y ep_limit_se) de tal forma que el jugador esté en el centro

	// Calculamos los límites de la zona visible tomando la posición del jugador como el centro
	player->ep_limit_nw.pos_x = (player_pos_x - max_game_x / 2) / CHUNK_N_TILES * CHUNK_N_TILES;
	player->ep_limit_nw.pos_y = (player_pos_y - max_game_y / 2) / CHUNK_N_TILES * CHUNK_N_TILES;

	player->ep_limit_se.pos_x = (player_pos_x + max_game_x / 2) / CHUNK_N_TILES * CHUNK_N_TILES;
	player->ep_limit_se.pos_y = (player_pos_y + max_game_y / 2) / CHUNK_N_TILES * CHUNK_N_TILES;
}

void draw_ui(int ticks_elapsed, double tps, char current_c) {
	wclear(info_win);

	box(info_win, 0, 0);
	mvwprintw(info_win, 1, 1, "SIZE_X: %d SIZE_Y: %d", getmaxx(stdscr), getmaxy(stdscr));
	mvwprintw(info_win, 2, 1, "TPS: %.2lf", tps);
	mvwprintw(info_win, 3, 1, "ELAPSED: %d", ticks_elapsed);
	mvwprintw(info_win, 4, 1, "KEY: %c", current_c);
	mvwprintw(info_win, 5, 1, "HP: %d", hp);
	mvwprintw(info_win, 6, 1, "POS X: %d", pj_generic->pos.pos_x);
	mvwprintw(info_win, 7, 1, "POS Y: %d", pj_generic->pos.pos_y);

	wrefresh(info_win);
}

void handle_input(int *game) {
	char current_c = getch();
	switch (current_c) {
		case 'w': case 'W': pj_generic->pos.pos_y++; break;
		case 's': case 'S': pj_generic->pos.pos_y--; break;
		case 'a': case 'A': pj_generic->pos.pos_x--; break;
		case 'd': case 'D': pj_generic->pos.pos_x++; break;
		case 'q': case 'Q': *game = 0; break;
		default: break;
	}
}



void draw_game_content(int ticks_elapsed) {
    int32_t i, j;
    int32_t start_x, start_y;

    // Calcular las posiciones globales iniciales (origen en la pantalla)
    start_x = pj_generic->pos.pos_x - (max_game_x / 2);
    start_y = pj_generic->pos.pos_y - (max_game_y / 2);

    // Iterar a través de cada tile visible en la pantalla
    for (i = 0; i < max_game_y; i++) {
        for (j = 0; j < max_game_x; j++) {
            // Calcular las coordenadas globales de cada tile
            int global_x = start_x + j;
            int global_y = start_y + (max_game_y - i - 1); // Invertir el eje Y

            // Determinar las coordenadas del chunk que contiene el tile
            int chunk_x = global_x / 16;
            int chunk_y = global_y / 16;

            // Ajustar coordenadas para números negativos
            if (global_x < 0) chunk_x = (global_x - 15) / 16;
            if (global_y < 0) chunk_y = (global_y - 15) / 16;

            // Calcular el índice del chunk en `client_chunks`
            int chunk_index = (chunk_x - (pj_generic->pos.pos_x / 16 - max_game_x / 32)) + 
                              (chunk_y - (pj_generic->pos.pos_y / 16 - max_game_y / 32)) * (max_game_x / 16);

            // Verificar si el índice del chunk es válido
            if (chunk_index < 0 || chunk_index >= (max_game_x / 16) * (max_game_y / 16)) {
                continue;
            }

            // Calcular las coordenadas del tile dentro del chunk
            int tile_x = (global_x % 16 + 16) % 16;
            int tile_y = (global_y % 16 + 16) % 16;

            // Verificar si el tile existe dentro del chunk
            if (!client_chunks[chunk_index].tile || !client_chunks[chunk_index].tile[tile_y]) {
                continue;
            }

            // Dibujar el bloque individual
            p_natural_block(client_chunks[chunk_index].tile[tile_y][tile_x], ticks_elapsed % 96, j, i);
        }
    }
}


void cleanup() {
	endwin();

	free(events_in);
	free(events_out);

	for (size_t i = 0; i < N_PLAYERS; i++) {
		free(players[i]);
	}
	free(players);
}

