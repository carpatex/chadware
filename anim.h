#ifndef ANIM_H
#define ANIM_H
#include "chadware.h"

typedef char** anim_frame;

struct anim_part {
	const int8_t n_frames;
	anim_frame *frame;
	uint8_t size_x;
	uint8_t size_y;
};

char** iterate_anim_part(struct anim_part, int8_t);


#endif
