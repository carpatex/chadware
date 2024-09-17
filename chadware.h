#ifndef CHADWARE_H
#define CHADWARE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct LoadedChunk {
	int32_t tile[16][16];
	int16_t extra_tile[16][16];
	int32_t chunk_version;
};
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

void compress_chunk32(struct LoadedChunk *, int *, int, struct PackedValues32 *);
void compress_chunk16(struct LoadedChunk *, int *, int, struct PackedValues16 *);
void decompress_chunk32(struct PackedValues32 *, int, struct LoadedChunk *);
void decompress_chunk16(struct PackedValues16 *, int, struct LoadedChunk *);

int tick(int, int *);
void *init_heap_chadware(size_t);
void debug_print_ptrs();

#endif
