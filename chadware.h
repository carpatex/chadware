#ifndef CHADWARE_H
#define CHADWARE_H
#define max(X, Y) ((X) > (Y) ? (X) : (Y))
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define PLAYER_NAME_SIZE 64
#define CHUNK_N_TILES 16
#define SUBMOTION_CONSTANT 16 
struct ChunkLocator {
	int16_t dimension; // dimensions: there are 3: -1 for HELL, 0 for SPACE, 1 for HEAVEN
	int32_t galaxy; // Galaxy, only applies literally to SPACE, but it can have another use on HEAVEN or HELL
	int32_t system; // same as above, in the case of SPACE it represents planetary systems which contain orbits.
	int8_t orbit; // same as above, in the case of SPACE it represents the orbits of planets, including the planet itself, satellites and asteroids.
	int32_t surface; // last mandatory layer, it can be satellites, asteroids, or planets.
	int32_t subsurface; // For vehicles only, 0 is the base surface. It shares entities with surface.
	int32_t start_pos_x;
	int32_t start_pos_y;
};

struct LoadedChunk {
	int32_t tile[CHUNK_N_TILES][CHUNK_N_TILES];
	int16_t extra_tile[CHUNK_N_TILES][CHUNK_N_TILES];
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
struct EntityIdentifier { // There can't be two identic instances of this in the same surface
	int32_t type;
	int32_t id;
	size_t entity_name_size;
	char* entity_name;
};
struct EntityPositionSurface {
	int32_t pos_x;
	int8_t motion_pos_x;
	int32_t pos_y;
	int8_t motion_pos_y;
};

struct EntityGeneric {
	struct ChunkLocator location;
	struct EntityIdentifier identifier;
	struct EntityPositionSurface pos;
	size_t size;
	void *data;
};
struct Item {
	int32_t id;
	int16_t n_modifier;
	size_t extra_data_size;
	void *extra_data;
};
struct EntityPlayer {
	int32_t max_hp;
	int32_t hp;
	int32_t xp;
	struct Item left_hand;
	struct Item right_hand;
	struct Item armor_head;
	struct Item armor_chest;
	struct Item armor_legs;
	struct Item armor_foot;
};
struct Event { //general struct for events. each particular type of event is pointed by the generic void 'data'
	int32_t event_id;
	size_t data_size;
	void *data;
};
struct MotionEvent { // struct for movement of entities &/or players
	struct ChunkLocator location;
	struct EntityIdentifier id;
	int32_t height_motion;
	int32_t x_motion;
	int8_t x_sub_motion;
	int32_t y_motion;
	int8_t y_sub_motion;
};

extern size_t heap_size;
extern size_t globalvar_size;
extern size_t datain_size;
extern size_t dataout_size;
extern size_t chunk_size;
extern size_t entity_generic_size;
extern size_t entity_size;

extern void *heap;
extern void **globalvar_ptr; 
extern void **v_datain_ptr;
extern void **v_dataout_ptr;
extern void **v_chunk_ptr;
extern void **v_entity_generic_ptr;
extern void **v_entity_ptr;
extern void **heapend_ptr;
extern int32_t curr_tick_epoch;
extern int32_t curr_tick;
extern struct EntityGeneric* entity_generic_ptr;

void compress_chunk32(struct LoadedChunk *, int *, int, struct PackedValues32 *);
void compress_chunk16(struct LoadedChunk *, int *, int, struct PackedValues16 *);
void decompress_chunk32(struct PackedValues32 *, int, struct LoadedChunk *);
void decompress_chunk16(struct PackedValues16 *, int, struct LoadedChunk *);

int32_t tick(int32_t, struct Event*, int32_t *, struct Event*);
int32_t handleMotionEvent(struct MotionEvent*);
size_t init_heap_chadware(size_t);
void debug_print_ptrs();

#endif
