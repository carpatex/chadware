#ifndef CHADWARE_H
#define CHADWARE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct Chunk {
	int32_t tile[16][16];
	int16_t extra_tile[16][16];
	int32_t chunk_version;
}; 
int tick(int, int *);
void* init_heap_chadware(size_t);
void debug_print_ptrs();

#endif
