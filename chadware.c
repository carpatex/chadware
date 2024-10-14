#include "chadware.h"
#define GLOBALVAR_PROPORTION 5
#define DATAIN_PROPORTION 10
#define DATAOUT_PROPORTION 10
#define CHUNK_PROPORTION 35
#define ENTITYGENERIC_PROPORTION 5
#define ENTITY_PROPORTION 35

size_t heap_size;
size_t globalvar_size;
size_t datain_size;
size_t dataout_size;
size_t chunk_size;
size_t entity_generic_size;
size_t entity_size;
void *heap;
void **globalvar_ptr; 
void **v_datain_ptr;
void **v_dataout_ptr;
void **v_chunk_ptr;
void **v_entity_generic_ptr;
void **v_entity_ptr;
void **heapend_ptr;
struct EntityGeneric* entity_generic_ptr;
// pointers to multiple sections of heap. A single pointer points to the only section of reserved memory.
// This is done to avoid having to reserve more memory each time the game needs more. 
int32_t curr_tick;
int32_t curr_tick_epoch;
size_t init_heap_chadware(const size_t intended_heap_size){
	if (((GLOBALVAR_PROPORTION * intended_heap_size) / 100) < (5 * sizeof(size_t))) {
		return 0; // This obviously means that memory couldn't be reserved.
	} else {
		globalvar_size = ((GLOBALVAR_PROPORTION * intended_heap_size) / 100) + ((GLOBALVAR_PROPORTION * intended_heap_size) % 100);
		datain_size = ((DATAIN_PROPORTION * intended_heap_size) / 100) + ((DATAIN_PROPORTION * intended_heap_size) % 100);
		dataout_size = ((DATAOUT_PROPORTION * intended_heap_size) / 100) + ((DATAOUT_PROPORTION * intended_heap_size) % 100);
		chunk_size = ((CHUNK_PROPORTION * intended_heap_size) / 100) + ((CHUNK_PROPORTION * intended_heap_size) % 100);
		entity_generic_size = ((ENTITYGENERIC_PROPORTION * intended_heap_size) / 100) + ((ENTITYGENERIC_PROPORTION * intended_heap_size) % 100);
		entity_size = ((ENTITY_PROPORTION * intended_heap_size) / 100) + ((ENTITY_PROPORTION * intended_heap_size) % 100);

		if (entity_generic_size % sizeof(struct EntityGeneric)) { // adding enough bytes if division between proportions aren't exact.
			entity_generic_size += sizeof(struct EntityGeneric) - entity_generic_size % sizeof(struct EntityGeneric);
		}
		
		heap_size = globalvar_size + datain_size + dataout_size + chunk_size + entity_generic_size + entity_size;

		heap = calloc(1, heap_size);
		if (heap == NULL) {
			return 0;
		}
		// The globalvar section has all global (i.e important for the whole game) variables. 
		// This includes all the pointers to the other sections and the current time of the game.
		heapend_ptr = heap;
		v_datain_ptr = heap + (2 * sizeof(void *));
		v_dataout_ptr = heap + (3 * sizeof(void *));
		v_chunk_ptr = heap + (4 * sizeof(void *));
		v_entity_generic_ptr = heap + (5 * sizeof(void *));
		v_entity_ptr = heap + (5 * sizeof(void *));

		*heapend_ptr = heap + heap_size;
		*globalvar_ptr = heap;
		*v_datain_ptr = heap + globalvar_size;
		*v_dataout_ptr = *v_datain_ptr + datain_size;
		*v_chunk_ptr = *v_dataout_ptr + dataout_size;
		*v_entity_generic_ptr = *v_chunk_ptr + chunk_size;
		*v_entity_ptr = *v_chunk_ptr + entity_generic_size;

		return heap_size;
	}
}
int init_chadware(int32_t n_players, char* player_names[]) {
	curr_tick_epoch = 0;
	curr_tick = 0;

	entity_generic_ptr = *v_entity_generic_ptr;
	int32_t i;
	if (sizeof(struct EntityGeneric) * max(n_players, ENTITYGENERIC_PROPORTION) <= *v_entity_ptr - *v_chunk_ptr) {
		// Check if there is enough space in the entity section to make the players.
		return 1;
	}
	for (i = 0; i < n_players; i++) {
		entity_generic_ptr[i].identifier.type = 0; // sets the entity type to 0 for EntityPlayer
		entity_generic_ptr[i].size = sizeof(struct EntityPlayer);
		entity_generic_ptr[i].data = *v_entity_ptr + i * sizeof(struct EntityPlayer);
	}
	// do world gen
	return 0;
}
int32_t tick(int32_t n_event_in, struct Event *event_in_list, int32_t *n_event_out, struct Event *event_out_list) {
	int32_t i;
	for (i = 0; i < n_event_in; i++) {
		switch (event_in_list[i].event_id) {
			case 0: // movement 
				handleMotionEvent((struct MotionEvent*) event_in_list[i].data);
			default:
				fprintf(stderr, "Invalid event id %d.\n", event_in_list[i].event_id);
		}
	}
	return 0;
}

void debug_print_ptrs() {
	printf("heapend_ptr %ld\n", *heapend_ptr);
	printf("globalvar_ptr %ld\n", *globalvar_ptr);
	printf("v_datain_ptr %ld\n", *v_datain_ptr);
	printf("v_dataout_ptr %ld\n", *v_dataout_ptr);
	printf("v_chunk_ptr %ld\n", *v_chunk_ptr);
	printf("v_entity_ptr %ld\n", *v_entity_ptr);
}
