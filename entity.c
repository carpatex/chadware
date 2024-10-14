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
int32_t handleMotionEvent(struct MotionEvent *ev) {
	struct EntityGeneric*  e;
	e = findEntity(&(ev)->location, &(ev)->id);
}
