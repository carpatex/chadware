#include "chadware.h"
#define NEW_ENTITIES_REALLOC 4

int add_entity(struct chunk*, struct entity) {
	int i, unused_entity_ocurrence = 0;
	for (i = 0; i < *(chunk)->n_entities; i++) {
		if (*(chunk)->*(entities + i)->hp) {
			continue;
		}
		else {
			unused_entity_ocurrence = i;
			break;
		}
	}
	if (unused_entity_ocurrence) {
		*(chunk)->*(entities + (size_t) unused_entity_ocurrence) = entity;
	}
	else {
		*(chunk)->entities = (struct entity *) realloc( (void *) *(chunk)->entities, (size_t) *(chunk)->n_entities + NEW_ENTITIES_REALLOC );
		if (chunk
		*(chunk)->*(entities + (size_t) *(chunk)->n_entities) = entity;
	}
	return 0;
}

