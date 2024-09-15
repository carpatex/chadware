#include "chadware.h"

#define CHUNK_N_TILES 16

struct PackedValues16 {
	int16_t value;
	int8_t start_pos_x;
	int8_t start_pos_y;
	int8_t end_pos_x;
	int8_t end_pos_y;
};
struct PackedValues32 {
	int32_t value;
	int8_t start_pos_x;
	int8_t start_pos_y;
	int8_t end_pos_x;
	int8_t end_pos_y;
};

void compress_chunk32(struct Chunk *chunk, int* packed_values_used, int max_packed_values, struct PackedValues32* result) {
	int i, j, start_x, start_y, end_x, end_y, count = 0;
	int32_t value;

	for (i = 0; i < CHUNK_N_TILES; i++) {
		for (j = 0; j < CHUNK_N_TILES; j++) {
			if (i == 0 || j == 0 || (chunk)->tile[i][j] != (chunk)->tile[i-1][j] || (chunk)->tile[i][j] != (chunk)->tile[i][j-1]) {
				start_x = i;
				start_y = j;
				end_x = i;
				end_y = j;
				value = (chunk)->tile[i][j];

				// Buscar coincidencias hacia abajo y derecha
				while (end_x < CHUNK_N_TILES && (chunk)->tile[end_x][j] == value) end_x++;
				while (end_y < CHUNK_N_TILES && (chunk)->tile[i][end_y] == value) end_y++;

				// Añadir coincidencia a resultado
				result[count].value = value;
				result[count].start_pos_x = start_x;
				result[count].start_pos_y = start_y;
				result[count].end_pos_x = end_x - 1;
				result[count].end_pos_y = end_y - 1;
				count++;
			}
		}
	}

	*packed_values_used = count;
}

void decompress_chunk32(struct PackedValues32* packed_values, int n_packed_values, struct Chunk *chunk) {
    int i, start_x, start_y, end_x, end_y;
	int32_t value;
    for (i = 0; i < n_packed_values; i++) {
		start_x = packed_values[i].start_pos_x;
		start_y = packed_values[i].start_pos_y;
		end_x = packed_values[i].end_pos_x;
		end_y = packed_values[i].end_pos_y;
		value = packed_values[i].value;

		int x, y;
		for (x = start_x; x <= end_x; x++) {
			for (y = start_y; y <= end_y; y++) {
				(chunk)->tile[x][y] = value;
			}
		}
	}
}
void compress_chunk16(struct Chunk *chunk, int* packed_values_used, int max_packed_values, struct PackedValues16* result) {
	int i, j, start_x, start_y, end_x, end_y, count = 0;
	int16_t value;

	for (i = 0; i < CHUNK_N_TILES; i++) {
		for (j = 0; j < CHUNK_N_TILES; j++) {
			if (i == 0 || j == 0 || (chunk)->tile[i][j] != (chunk)->tile[i-1][j] || (chunk)->tile[i][j] != (chunk)->tile[i][j-1]) {
				start_x = i;
				start_y = j;
				end_x = i;
				end_y = j;
				value = (chunk)->extra_tile[i][j];

				// Buscar coincidencias hacia abajo y derecha
				while (end_x < CHUNK_N_TILES && (chunk)->tile[end_x][j] == value) end_x++;
				while (end_y < CHUNK_N_TILES && (chunk)->tile[i][end_y] == value) end_y++;

				// Añadir coincidencia a resultado
				result[count].value = value;
				result[count].start_pos_x = start_x;
				result[count].start_pos_y = start_y;
				result[count].end_pos_x = end_x - 1;
				result[count].end_pos_y = end_y - 1;
				count++;
			}
		}
	}

	*packed_values_used = count;
}

void decompress_chunk16(struct PackedValues16* packed_values, int n_packed_values, struct Chunk *chunk) {
    int x, y, i, start_x, start_y, end_x, end_y;
	int16_t value;
    for (i = 0; i < n_packed_values; i++) {
		start_x = packed_values[i].start_pos_x;
		start_y = packed_values[i].start_pos_y;
		end_x = packed_values[i].end_pos_x;
		end_y = packed_values[i].end_pos_y;
		value = packed_values[i].value;
		for (x = start_x; x <= end_x; x++) {
			for (y = start_y; y <= end_y; y++) {
				(chunk)->tile[x][y] = value;
			}
		}
	}
}
