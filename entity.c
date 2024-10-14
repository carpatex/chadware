#include "chadware.h"
#define NEW_ENTITIES_REALLOC 4
struct EntityGeneric* findEntity(struct ChunkLocator* location, struct EntityIdentifier* identifier) {
	int i;
	for (i = 0; i < entity_generic_size / sizeof(struct EntityGeneric); i++) {
		if (!memcmp(location, &entity_generic_ptr[i].location, sizeof(struct ChunkLocator)) && !memcmp(identifier, &entity_generic_ptr[i].identifier, sizeof(struct EntityIdentifier))) {
			return &entity_generic_ptr[i];
		}
	}
	return NULL;
}

void moveEntity(int32_t x_motion, int8_t x_sub_motion, int32_t y_motion, int8_t y_sub_motion, struct EntityPositionSurface* eps_ptr) {
	int32_t i;
	int8_t x_sub_motion_r = (x_sub_motion + eps_ptr->motion_pos_x) % SUBMOTION_CONSTANT;
	int8_t y_sub_motion_r = (y_sub_motion + eps_ptr->motion_pos_y) % SUBMOTION_CONSTANT;
	eps_ptr->pos_x += x_motion + (int32_t) ((x_sub_motion + eps_ptr->motion_pos_x) / SUBMOTION_CONSTANT);
	eps_ptr->pos_y += y_motion;
}

int32_t handleMotionEvent(struct MotionEvent *ev) {
	struct EntityGeneric* e = findEntity(&(ev)->location, &(ev)->id);
	switch((ev)->id.id) {
		case 0: // a player
			break;
		default:
	}
}
