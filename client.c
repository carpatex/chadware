
#include "chadware.h"
#include "chadgraphics.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
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
int32_t pos_x, pos_y, hp;

// Function declarations
void initialize_memory();
void initialize_game();
void setup_ncurses();
void cleanup();
void handle_resize(int *size_x, int *size_y);
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

        handle_resize(&size_x, &size_y);

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

void handle_resize(int *size_x, int *size_y) {
    static int old_size_x = 0, old_size_y = 0;
    getmaxyx(stdscr, *size_y, *size_x);

    if (*size_x != old_size_x || *size_y != old_size_y) {
        resizeterm(*size_y, *size_x);
        info_win = newwin(*size_y, INFO_WIDTH, 0, *size_x - INFO_WIDTH);
        main_win = newwin(*size_y, *size_x - INFO_WIDTH, 0, 0);
        old_size_x = *size_x;
        old_size_y = *size_y;
    }

    getmaxyx(main_win, max_game_y, max_game_x);
    getbegyx(main_win, beg_game_y, beg_game_x);
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
        case 'w': case 'W': pj_generic->pos.pos_y+=3; break;
        case 's': case 'S': pj_generic->pos.pos_y-=3; break;
        case 'a': case 'A': pj_generic->pos.pos_x-=3; break;
        case 'd': case 'D': pj_generic->pos.pos_x+=3; break;
        case 'q': case 'Q': *game = 0; break;
        default: break;
    }
}


void draw_game_content(GameState *gameState) {
    for (int i = 0; i < gameState->chunks_count; i++) {
        struct LoadedChunk *chunk = &gameState->chunks[i];
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                int tile_id = chunk->tiles[x][y];
                p_natural_block(tile_id, 0, chunk->location.x + x, chunk->location.y + y);
            }
        }
    }
}

    }

    // Draw blocks with inverted Y-axis
    for (i = 0; i < max_game_y; i++) {
        for (j = 0; j < max_game_x; j++) {
            // Calculate global coordinates with inverted Y-axis
            int global_x = start_chunk_x * 16 + j;
            int global_y = start_chunk_y * 16 + (max_game_y - i - 1); // Invert Y-axis

            // Calculate the index of the corresponding chunk
            int chunk_x_index = (global_x >= 0 ? global_x / 16 : (global_x - 15) / 16) - start_chunk_x;
            int chunk_y_index = (global_y >= 0 ? global_y / 16 : (global_y - 15) / 16) - start_chunk_y;

            chunk_index = chunk_x_index + chunk_y_index * max_chunk_x;

            // Verify chunk index is within valid range
            if (chunk_index < 0 || chunk_index >= max_chunk_x * max_chunk_y) {
                continue;
            }

            // Calculate tile positions within the chunk
            tile_x = ((global_x % 16) + 16) % 16;
            tile_y = ((global_y % 16) + 16) % 16;

            // Verify if the tile exists
            if (!lchunk_ptr[chunk_index].tile || !lchunk_ptr[chunk_index].tile[tile_y]) {
                continue;
            }

            // Draw the block with inverted vertical position
            p_natural_block(lchunk_ptr[chunk_index].tile[tile_y][tile_x], current_tick % 96, j, i);
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

