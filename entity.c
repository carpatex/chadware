#include "chadware.h"
#define NEW_ENTITIES_REALLOC 4
struct EntityGeneric* findEntity(struct Locator* location, struct EntityIdentifier* identifier) {
	int i;
	for (i = 0; i < entityg_size / sizeof(struct EntityGeneric); i++) {
		if (!memcmp(location, &entityg_ptr[i].location, sizeof(struct Locator)) && !memcmp(identifier, &entityg_ptr[i].identifier, sizeof(struct EntityIdentifier))) {
			return &entityg_ptr[i];
		}
	}
	return NULL;
}

void moveEntity(int32_t x_motion, int8_t x_sub_motion, int32_t y_motion, int8_t y_sub_motion, struct EntityPositionSurface* eps_ptr) {
	int32_t i;


	if((x_sub_motion + eps_ptr->motion_pos_x) % SUBMOTION_CONSTANT < 0) {
		int8_t x_sub_motion_r = 0 - ((x_sub_motion + eps_ptr->motion_pos_x) % SUBMOTION_CONSTANT);
	}
	else {
		int8_t x_sub_motion_r = (x_sub_motion + eps_ptr->motion_pos_x) % SUBMOTION_CONSTANT;
	}

	if((y_sub_motion + eps_ptr->motion_pos_y) % SUBMOTION_CONSTANT < 0) {
		int8_t y_sub_motion_r = 0 - ((y_sub_motion + eps_ptr->motion_pos_y) % SUBMOTION_CONSTANT);
	}
	else {
		int8_t y_sub_motion_r = (y_sub_motion + eps_ptr->motion_pos_y) % SUBMOTION_CONSTANT;
	}

	eps_ptr->pos_x += x_motion + (int32_t) ((x_sub_motion + eps_ptr->motion_pos_x) / SUBMOTION_CONSTANT);
	eps_ptr->pos_y += y_motion + (int32_t) ((y_sub_motion + eps_ptr->motion_pos_y) / SUBMOTION_CONSTANT);
}

void handleMotionEvent(struct MotionEvent *ev) {
	struct EntityGeneric* e = findEntity(&(ev)->location, &(ev)->id);
	switch(ev->id.id) {
		case 0: // a player
			moveEntity(ev->x_motion, ev->x_sub_motion, ev->y_motion, ev->y_sub_motion, &e->pos);
			break;
		default:
			moveEntity(ev->x_motion, ev->x_sub_motion, ev->y_motion, ev->y_sub_motion, &e->pos);
	}
}
