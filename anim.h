#ifndef ANIM_H
#define ANIM_H
#include "chadware.h"

typedef char** anim_frame;

typedef struct {
	const int8_t n_frames;
	anim_frame *frame;
	uint8_t size_x;
	uint8_t size_y;
} anim_part;

char** iterate_anim_part(anim_part, int8_t);


#endif
