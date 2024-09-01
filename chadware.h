#ifndef CHADWARE_H
#define CHADWARE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct chunk {
	int32_t tile[16][16];
	int16_t extra_tile[16][16];
	void** entity;
	int32_t chunk_version;
}; 

void* init_heap_chadware(size_t);
void debug_print_ptrs();

#endif
