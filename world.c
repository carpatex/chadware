#include "chadware.h"

struct LoadedChunk* find_chunk(int32_t x, int32_t y, struct Locator loc, struct LoadedChunk* chunks, size_t n_chunks) {
	for (size_t i = 0; i < n_chunks; i++) {
		struct LoadedChunk* chunk = &chunks[i];
		if (chunk->location.dimension == loc.dimension &&
				chunk->location.galaxy == loc.galaxy &&
				chunk->location.planetary_system == loc.planetary_system &&
				chunk->location.orbit == loc.orbit &&
				chunk->location.surface == loc.surface &&
				chunk->location.subsurface == loc.subsurface &&
				chunk->start_pos_x <= x && x < chunk->start_pos_x + CHUNK_N_TILES &&
				chunk->start_pos_y <= y && y < chunk->start_pos_y + CHUNK_N_TILES) {
			return chunk;
		}
	}
	return NULL; // No se encontró el chunk
}
struct ToplevelTerrain* findToplevel(struct Locator *location) {
	size_t i;
	for (i = 0; i < n_toplevel_terrain; i++) {
		if (!memcmp(location, &toplevel_terrain_ptr[i].hierarchy, sizeof(struct Locator)))
			return &toplevel_terrain_ptr[i];
	}
	return NULL;
}

void compress_chunk32(struct LoadedChunk *chunk, int16_t *packed_values_used, int16_t max_packed_values, struct PackedValues32 *result) {
	int16_t i, j, start_x, start_y, end_x, end_y, count = 0;
	int32_t value;
	for (i = 0; i < CHUNK_N_TILES; i++) {
		for (j = 0; j < CHUNK_N_TILES; j++) {
			if (i == 0 || j == 0 || (chunk)->tile[i][j] != (chunk)->tile[i - 1][j] || (chunk)->tile[i][j] != (chunk)->tile[i][j - 1]) {
				start_x = i;
				start_y = j;
				end_x = i;
				end_y = j;
				value = (chunk)->tile[i][j];

				// Buscar coincidencias hacia abajo y derecha
				while (end_x < CHUNK_N_TILES && (chunk)->tile[end_x][j] == value)
					end_x++;
				while (end_y < CHUNK_N_TILES && (chunk)->tile[i][end_y] == value)
					end_y++;

				// Añadir coincidencia a resultado
				result[count].value = value;
				result[count].start_pos_x = start_x;
				result[count].start_pos_y = start_y;
				result[count].end_pos_x = end_x - 1;
				result[count].end_pos_y = end_y - 1;
				if (count >= max_packed_values) {
					*packed_values_used = count;
					return;
				} else {
					count++;
				}
			}
		}
	}
	*packed_values_used = count;
}

void decompress_chunk32(struct PackedValues32 *packed_values, int16_t n_packed_values, struct LoadedChunk *chunk) {
	int16_t i, start_x, start_y, end_x, end_y;
	int32_t value;
	for (i = 0; i < n_packed_values; i++) {
		start_x = packed_values[i].start_pos_x;
		start_y = packed_values[i].start_pos_y;
		end_x = packed_values[i].end_pos_x;
		end_y = packed_values[i].end_pos_y;
		value = packed_values[i].value;

		int16_t x, y;
		for (x = start_x; x <= end_x; x++) {
			for (y = start_y; y <= end_y; y++) {
				(chunk)->tile[x][y] = value;
			}
		}
	}
}
void compress_chunk16(struct LoadedChunk *chunk, int16_t *packed_values_used, int16_t max_packed_values, struct PackedValues16 *result) {
	int16_t i, j, start_x, start_y, end_x, end_y, count = 0;
	int16_t value;

	for (i = 0; i < CHUNK_N_TILES; i++) {
		for (j = 0; j < CHUNK_N_TILES; j++) {
			if (i == 0 || j == 0 || (chunk)->extra_tile[i][j] != (chunk)->extra_tile[i - 1][j] ||
					(chunk)->extra_tile[i][j] != (chunk)->extra_tile[i][j - 1]) {
				start_x = i;
				start_y = j;
				end_x = i;
				end_y = j;
				value = (chunk)->extra_tile[i][j];

				// Buscar coincidencias hacia abajo y derecha
				while (end_x < CHUNK_N_TILES && (chunk)->extra_tile[end_x][j] == value)
					end_x++;
				while (end_y < CHUNK_N_TILES && (chunk)->extra_tile[i][end_y] == value)
					end_y++;

				// Añadir coincidencia a resultado
				result[count].value = value;
				result[count].start_pos_x = start_x;
				result[count].start_pos_y = start_y;
				result[count].end_pos_x = end_x - 1;
				result[count].end_pos_y = end_y - 1;
				if (count >= max_packed_values) {
					*packed_values_used = count;
					return;
				} else {
					count++;
				}
			}
		}
	}
	*packed_values_used = count;
}

void decompress_chunk16(struct PackedValues16 *packed_values, int16_t n_packed_values, struct LoadedChunk *chunk) {
	int16_t x, y, i, start_x, start_y, end_x, end_y;
	int16_t value;
	for (i = 0; i < n_packed_values; i++) {
		start_x = packed_values[i].start_pos_x;
		start_y = packed_values[i].start_pos_y;
		end_x = packed_values[i].end_pos_x;
		end_y = packed_values[i].end_pos_y;
		value = packed_values[i].value;
		for (x = start_x; x <= end_x; x++) {
			for (y = start_y; y <= end_y; y++) {
				(chunk)->extra_tile[x][y] = value;
			}
		}
	}
}
