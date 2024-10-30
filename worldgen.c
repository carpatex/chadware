#include "chadware.h"
static int32_t locator_hash;
float interpolate(float a0, float a1, float w) {
	/* // You may want clamping by inserting:
	 * if (0.0 > w) return a0;
	 * if (1.0 < w) return a1;
	 */
	return (a1 - a0) * w + a0;
	/* // Use this cubicint32_terpolation [[Smoothstep]] instead, for a smooth appearance:
	 * return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
	 *
	 * // Use [[Smootherstep]] for an even smoother result with a second derivative equal to zero on boundaries:
	 * return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
	 */
}

struct float_coords {
	float x, y;
};

/* Create pseudorandom direction vector
*/
struct float_coords randomGradient(int32_t ix, int32_t iy) {
	// No precomputed gradients mean this works for any number of grid coordinates
	const uint32_t w = 8 * sizeof(uint32_t);
	const uint32_t s = w / 2; // rotation width
	uint32_t a = ix, b = iy;
	a *= 4206942069; b ^= a << s | a >> w-s;
	b *= locator_hash; a ^= b << s | b >> w-s;
	a *= 6942069420;
	float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
	struct float_coords v;
	v.x = cos(random); v.y = sin(random);
	return v;
}

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int32_t ix, int32_t iy, float x, float y) {
	// Get gradient fromint32_teger coordinates
	struct float_coords gradient = randomGradient(ix, iy);

	// Compute the distance vector
	float dx = x - (float)ix;
	float dy = y - (float)iy;

	// Compute the dot-product
	return (dx*gradient.x + dy*gradient.y);
}

// Compute Perlin noise at coordinates x, y
float perlin(float x, float y, struct Locator l) {
	// Determine grid cell coordinates
	locator_hash = seed ^ (l.galaxy ^ (l.planetary_system ^ (l.orbit ^ (l.surface ^ l.subsurface))));
	int32_t x0 = (int32_t)floor(x);
	int32_t x1 = x0 + 1;
	int32_t y0 = (int32_t)floor(y);
	int32_t y1 = y0 + 1;

	// Determineint32_terpolation weights
	// Could also use higher order polynomial/s-curve here
	float sx = x - (float)x0;
	float sy = y - (float)y0;

	// Interpolate between grid point gradients
	float n0, n1, ix0, ix1, value;

	n0 = dotGridGradient(x0, y0, x, y);
	n1 = dotGridGradient(x1, y0, x, y);
	ix0 = interpolate(n0, n1, sx);

	n0 = dotGridGradient(x0, y1, x, y);
	n1 = dotGridGradient(x1, y1, x, y);
	ix1 = interpolate(n0, n1, sx);

	value = interpolate(ix0, ix1, sy);
	return value; // Will return in range -1 to 1. To make it in range 0 to 1, multiply by 0.5 and add 0.5
}
void gen_spawn_areas(int32_t n_players) {

}
