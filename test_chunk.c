#include "chadware.h"
#include <unistd.h>
void printdigitspace(int32_t var) {
	if (!(var / 10)) {
		printf("%d   ", var);
	}
	else if (!(var / 100)) {
		printf("%d  ", var);
	}
	else if (!(var / 1000)) {
		printf("%d ", var);
	}
}
int main(){
	int i, j, curr_noise;
	srand(time(NULL) ^ ~clock());
	int32_t seed = rand();
	struct LoadedChunk ch;
	ch.location.dimension = 0;
	ch.location.galaxy = 1;
	ch.location.planetary_system = 1;
	ch.location.orbit = 4;
	ch.location.surface = 0;
	ch.location.subsurface = 0;
	ch.status = 0;
	ch.start_pos_x = 128;
	ch.start_pos_y = -64;
	while (1) {
		for (i = 0; i < CHUNK_N_TILES; i++) {
			for(j = 0; j < CHUNK_N_TILES; j++) {
				ch.tile[j][i] = 0;
				ch.extra_tile[j][i] = 0;
			}
		}
		ch.start_pos_x += 16;
		gen_chunk(560134244, &ch);
		for (i = 0; i < CHUNK_N_TILES; i++) {
			for(j = 0; j < CHUNK_N_TILES; j++) {
				// if (ch.tile[j][i] == 48)   // solo mostrar agua
				//if (ch.tile[j][i] <= 8 && ch.tile[j][i] >= 1) // solo mostrar pasto
				//if (ch.tile[j][i] <= 32 && ch.tile[j][i] >= 25) // solo mostrar arena
				/*else
					printf("   "); */
				printdigitspace(ch.tile[j][i]);
			}
			putchar('\n');
		}
		sleep(1);
	}
	return 0;
}
