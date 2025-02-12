#include "chadware.h"
#define GLOBALVAR_PROPORTION 5
#define DATAIN_PROPORTION 10
#define DATAOUT_PROPORTION 10
#define TOPLEVEL_TERRAIN_PROPORTION 10 
#define CHUNK_PROPORTION 25
#define ENTITYGENERIC_PROPORTION 5
#define ENTITY_PROPORTION 35

size_t heap_size;
size_t eventg_in_size;
size_t eventg_out_size;
size_t toplevel_terrain_size;
size_t chunk_size;
size_t entityg_size;
size_t entity_size;

size_t n_eventg_in;
size_t n_eventg_out;
size_t n_toplevel_terrain;
size_t n_entityg;
size_t n_chunks;

void *heap;
void *v_eventg_in_ptr;
void *v_eventg_out_ptr;
void *v_toplevel_terrain_ptr;
void *v_chunk_ptr;
void *v_entityg_ptr;
void *v_entity_ptr;
void *heapend_ptr;

struct EventGeneric* eventg_in_ptr;
struct EventGeneric* eventg_out_ptr;
struct LoadedChunk* lchunk_ptr;
struct ToplevelTerrain* toplevel_terrain_ptr;
struct EntityGeneric* entityg_ptr;
// pointers to multiple sections of heap. A single pointer points to the only section of reserved memory.
// This is done to avoid having to reserve more memory each time the game needs more. 
uint32_t curr_tick;
uint32_t curr_tick_epoch;
int32_t seed;

size_t init_heap_chadware(const size_t intended_heap_size){
	eventg_in_size = ((DATAIN_PROPORTION * intended_heap_size) / 100) + ((DATAIN_PROPORTION * intended_heap_size) % 100);
	eventg_out_size = ((DATAOUT_PROPORTION * intended_heap_size) / 100) + ((DATAOUT_PROPORTION * intended_heap_size) % 100);
	chunk_size = ((CHUNK_PROPORTION * intended_heap_size) / 100) + ((CHUNK_PROPORTION * intended_heap_size) % 100);
	entityg_size = ((ENTITYGENERIC_PROPORTION * intended_heap_size) / 100) + ((ENTITYGENERIC_PROPORTION * intended_heap_size) % 100);
	entity_size = ((ENTITY_PROPORTION * intended_heap_size) / 100) + ((ENTITY_PROPORTION * intended_heap_size) % 100);


	if (eventg_in_size % sizeof(struct EventGeneric)) { // adding enough bytes if division between proportions aren't exact.
		eventg_in_size += sizeof(struct EventGeneric) - eventg_in_size % sizeof(struct EventGeneric);
	}
	if (eventg_out_size % sizeof(struct EventGeneric)) { // adding enough bytes if division between proportions aren't exact.
		eventg_out_size += sizeof(struct EventGeneric) - eventg_out_size % sizeof(struct EventGeneric);
	}
	if(chunk_size % sizeof(struct LoadedChunk)) {
		chunk_size += sizeof(struct LoadedChunk) - chunk_size % sizeof(struct LoadedChunk);
	}
	if (toplevel_terrain_size % sizeof(struct ToplevelTerrain)) { // adding enough bytes if division between proportions aren't exact.
		toplevel_terrain_size += sizeof(struct ToplevelTerrain) - toplevel_terrain_size % sizeof(struct ToplevelTerrain);
	}
	if (entityg_size % sizeof(struct EntityGeneric)) { // adding enough bytes if division between proportions aren't exact.
		entityg_size += sizeof(struct EntityGeneric) - entityg_size % sizeof(struct EntityGeneric);
	}


	heap_size = eventg_in_size + eventg_out_size + toplevel_terrain_size + chunk_size + entityg_size + entity_size;

	heap = calloc(1, heap_size);
	if (heap == NULL) {
		return 0;
	}
	// The globalvar section has all global (i.e important for the whole game) variables. 
	// This includes all the pointers to the other sections and the current time of the game.
	heapend_ptr = heap;

	heapend_ptr = heap + heap_size;
	v_eventg_in_ptr = heap;
	v_eventg_out_ptr = v_eventg_in_ptr + eventg_in_size;
	v_chunk_ptr = v_eventg_out_ptr + eventg_out_size;
	v_entityg_ptr = v_chunk_ptr + chunk_size;
	v_entity_ptr = v_chunk_ptr + entityg_size;

	return heap_size;
}
int init_chadware(int32_t n_players, char* player_names[]) {
	curr_tick_epoch = 0;
	curr_tick = 0;
	eventg_in_ptr = (struct EventGeneric*) v_eventg_in_ptr;
	n_eventg_in = eventg_in_size / sizeof(struct EventGeneric);
	eventg_out_ptr = (struct EventGeneric*) v_eventg_out_ptr;
	n_eventg_out = eventg_out_size / sizeof(struct EventGeneric);
	lchunk_ptr = (struct LoadedChunk *) v_chunk_ptr;
	n_chunks = chunk_size / sizeof(struct LoadedChunk);
	toplevel_terrain_ptr = (struct ToplevelTerrain*) v_toplevel_terrain_ptr;
	n_toplevel_terrain = toplevel_terrain_size / sizeof(struct ToplevelTerrain);
	entityg_ptr = (struct EntityGeneric*) v_entityg_ptr;
	n_entityg = entityg_size / sizeof(struct EntityGeneric);

	int32_t i;
	for (i = 0; i < n_players; i++) {
		entityg_ptr[i].identifier.type = 0; // sets the entity type to 0 for EntityPlayer
		entityg_ptr[i].size = sizeof(struct EntityPlayer);
		entityg_ptr[i].data = v_entity_ptr + i * sizeof(struct EntityPlayer);
	}
	srand(time(NULL) ^ ~(clock()));
	seed = ~(rand() ^ (rand() ^ (rand() << 4)));
	//gen_spawn_areas(n_players);
	// do world gen
	return 0;
}
void handleGenChunkEvent(struct GenChunkEvent *event) {
	int32_t chunk_count = 0;

	// Localizar al jugador
	struct EntityGeneric *player_entity = &entityg_ptr[event->entity_index];
	if (!player_entity || !player_entity->data) {
		fprintf(stderr, "Entidad de jugador no válida en índice %d.\n", event->entity_index);
		return;
	}

	// Obtener datos específicos del jugador
	struct EntityPlayer *player = (struct EntityPlayer *)player_entity->data;

	// Calcular límites en coordenadas de "chunk"
	int32_t start_chunk_x = player->ep_limit_nw.pos_x / CHUNK_N_TILES;
	int32_t start_chunk_y = player->ep_limit_nw.pos_y / CHUNK_N_TILES;
	int32_t end_chunk_x = player->ep_limit_se.pos_x / CHUNK_N_TILES;
	int32_t end_chunk_y = player->ep_limit_se.pos_y / CHUNK_N_TILES;

	// Generar "chunks" dentro de los límites del jugador
	for (int y = start_chunk_y; y <= end_chunk_y; y++) {
		for (int x = start_chunk_x; x <= end_chunk_x; x++) {
			if (chunk_count >= event->target_size) {
				fprintf(stderr, "Se superó el tamaño máximo de 'target'.\n");
				return;
			}

			// Verificar si el chunk ya existe usando findChunk
			if (findChunk(x, y, player_entity->location, event->target, chunk_count)) {
				continue; // Chunk ya cargado, saltar
			}

			struct LoadedChunk *current_chunk = &event->target[chunk_count];

			// Inicializar coordenadas y datos del chunk
			current_chunk->start_pos_x = x * CHUNK_N_TILES;
			current_chunk->start_pos_y = y * CHUNK_N_TILES;
			current_chunk->location = player_entity->location;

			// Generar el chunk
			gen_chunk(seed, current_chunk);

			// Validar el chunk generado
			if (!current_chunk->tile) {
				fprintf(stderr, "El chunk generado en (%d, %d) no contiene datos.\n", x, y);
				continue;
			}

			chunk_count++;
		}
	}
}
int32_t tick(int32_t n_event_in, struct EventGeneric *event_in_list, int32_t *n_event_out, struct EventGeneric *event_out_list) {
	int32_t i;
	for (i = 0; i < n_event_in; i++) {
		switch (event_in_list[i].event_id) {
			case -1: // Termination event
				free(heap);
				return 1;

			case 2: // Movement event
				handleMotionEvent((struct MotionEvent *) event_in_list[i].data);
				break;

			case 3: // Chunk generation event
				handleGenChunkEvent((struct GenChunkEvent *) event_in_list[i].data);
				break;

			default: // Invalid event
				fprintf(stderr, "Invalid event id %d.\n", event_in_list[i].event_id);
				break;
		}
	}

	// Update the tick and epoch if necessary
	if (curr_tick == UINT32_MAX) {
		curr_tick = 0;
		curr_tick_epoch++;
	}

	return 0;
}
