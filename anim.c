#include "anim.h"

struct anim_frame iterate_anim_part(struct anim_part part, const int8_t curr_n_frame) {
	int8_t actual_n_frame = (curr_n_frame + 1) % tile.m_frames;
	return *(part.tile + ((size_t) actual_n_frame * sizeof(void*));
}
