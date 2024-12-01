#ifndef CHADWARE_H
#define CHADWARE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <tgmath.h>

#define max(X, Y) ((X) > (Y) ? (X) : (Y))
#define min(X, Y) ((X) < (Y) ? (X) : (Y))

#define TPS_OBJECTIVE 32
#define NEXT_TICK CLOCKS_PER_SEC / TPS_OBJECTIVE 
#define PLAYER_NAME_SIZE 64
#define CHUNK_N_TILES 16
#define SUBMOTION_CONSTANT 16 

struct Locator { 
	int8_t dimension; // dimensions: there are 3: -1 for HELL, 0 for SPACE, 1 for HEAVEN
	uint32_t galaxy; // Galaxy, only applies literally to SPACE, but it can have another use on HEAVEN or HELL
	uint32_t planetary_system; // same as above, in the case of SPACE it represents planetary systems which contain orbits.
	uint32_t orbit; // same as above, in the case of SPACE it represents the orbits of planets, including the planet itself, satellites and asteroids.
	uint32_t surface; // last mandatory layer, it can be satellites, asteroids, or planets.
	uint32_t subsurface; // For interiors and vehicles only, 0 is the base surface. It shares entities with surface.
	};
struct ToplevelTerrain {	
	int8_t hierarchy_type; // the hierarchy type (0 for dimension, 1 for galaxy, 
												 // 2 for planetary_system and so on)
	struct Locator hierarchy;
	
	int32_t w_limit_x; // western limit for the x axis
	int32_t n_limit_y; // northern limit for the Y axis
	int32_t e_limit_x; // eastern limit for the X axis 
	int32_t s_limit_y; // southern limit for the Y axis

	uint32_t n_sublevels;
};
struct LoadedChunk {
	int32_t tile[CHUNK_N_TILES][CHUNK_N_TILES];
	int16_t extra_tile[CHUNK_N_TILES][CHUNK_N_TILES];
	struct Locator location;
	int8_t status;
	int32_t start_pos_x;
	int32_t start_pos_y;
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
struct NPC {
	int8_t iq;
	int32_t civ;
	int16_t role;
};
struct EntityPositionSurface {
	int32_t pos_x;
	int8_t motion_pos_x;
	int32_t pos_y;
	int8_t motion_pos_y;
};

struct EntityGeneric {
	struct Locator location;
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
struct EventGeneric { //general struct for events. each particular type of event is pointed by the generic void 'data'
	int32_t event_id;
	size_t data_size;
	void *data;
};
struct MotionEvent { // struct for movement of entities &/or players
	struct EntityGeneric *target;
	int32_t height_motion;
	int32_t x_motion;
	int8_t x_sub_motion;
	int32_t y_motion;
	int8_t y_sub_motion;
};

extern size_t heap_size;
extern size_t eventg_in_size;
extern size_t eventg_out_size;
extern size_t toplevel_terrain_size;
extern size_t chunk_size;
extern size_t entityg_size;
extern size_t entity_size;

extern size_t n_eventg_in;
extern size_t n_eventg_out;
extern size_t n_chunks;
extern size_t n_toplevel_terrain;
extern size_t n_entityg;

extern void *heap;
extern void *v_eventg_in_ptr;
extern void *v_eventg_out_ptr;
extern void *v_toplevel_terrain_ptr;
extern void *v_chunk_ptr;
extern void *v_entityg_ptr;
extern void *v_entity_ptr;
extern void *heapend_ptr;

extern struct EventGeneric* eventg_in_ptr;
extern struct EventGeneric* eventg_out_ptr;
extern struct LoadedChunk* lchunk_ptr;
extern struct ToplevelTerrain* toplevel_terrain_ptr;
extern struct EntityGeneric* entityg_ptr;

extern uint32_t curr_tick_epoch;
extern uint32_t curr_tick;

void compress_chunk32(struct LoadedChunk *, int16_t *, int16_t, struct PackedValues32 *);
void compress_chunk16(struct LoadedChunk *, int16_t *, int16_t, struct PackedValues16 *);
void decompress_chunk32(struct PackedValues32 *, int16_t, struct LoadedChunk *);
void decompress_chunk16(struct PackedValues16 *, int16_t, struct LoadedChunk *);

void gen_spawn_areas(int32_t);
void gen_chunk(int32_t, struct LoadedChunk *);
int32_t perlinint32(int32_t, int32_t, uint32_t); 

int32_t tick(int32_t, struct EventGeneric*, int32_t *, struct EventGeneric*);
void handleMotionEvent(struct MotionEvent*);
size_t init_heap_chadware(size_t);
int init_chadware(int32_t, char*[]); 

#endif
