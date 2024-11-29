#include "chadware.h"
static int32_t noise_hash[] = {208, 34, 231, 213, 32, 248, 233, 56, 161, 78, 24, 140, 71, 48, 140, 254, 245, 255, 247, 247, 40,
	185, 248, 251, 245, 28, 124, 204, 204, 76, 36, 1, 107, 28, 234, 163, 202, 224, 245, 128, 167, 204,
	9, 92, 217, 54, 239, 174, 173, 102, 193, 189, 190, 121, 100, 108, 167, 44, 43, 77, 180, 204, 8, 81,
	70, 223, 11, 38, 24, 254, 210, 210, 177, 32, 81, 195, 243, 125, 8, 169, 112, 32, 97, 53, 195, 13,
	203, 9, 47, 104, 125, 117, 114, 124, 165, 203, 181, 235, 193, 206, 70, 180, 174, 0, 167, 181, 41,
	164, 30, 116, 127, 198, 245, 146, 87, 224, 149, 206, 57, 4, 192, 210, 65, 210, 129, 240, 178, 105,
	228, 108, 245, 148, 140, 40, 35, 195, 38, 58, 65, 207,215, 253, 65, 85, 208, 76, 62, 3, 237, 55, 89,
	232, 50, 217, 64, 244, 157, 199, 121, 252, 90, 17, 212, 203, 149, 152, 140, 187, 234, 177, 73, 174,
	193, 100, 192, 143, 97, 53, 145, 135, 19, 103, 13, 90,135, 151, 199, 91, 239, 247, 33, 39, 145,
	101, 120, 99, 3, 186, 86, 99, 41, 237, 203, 111, 79, 220, 135, 158, 42, 30, 154, 120, 67, 87, 167,
	135, 176, 183, 191, 253, 115, 184, 21, 233, 58, 129, 233, 142, 39, 128, 211, 118, 137, 139, 255,
	114, 20, 218, 113, 154, 27, 127, 246, 250, 1, 8, 198, 250, 209, 92, 222, 173, 21, 88, 102, 219};

int32_t noise2(int32_t x, int32_t y, uint32_t full_seed)
{
	int32_t tmp = noise_hash[(y + full_seed) % 256];
	return noise_hash[(tmp + x) % 256];
}

float lin_inter(float x, float y, float s)
{
	return x + s * (y - x);
}

float smooth_inter(float x, float y, float s)
{
	return lin_inter(x, y, s * s * (3 - 2 * s));
}

float noise2d(float x, float y, uint32_t full_seed)
{
	int32_t x_int = x;
	int32_t y_int = y;
	float x_frac = x - x_int;
	float y_frac = y - y_int;
	int32_t s = noise2(x_int, y_int, full_seed);
	int32_t t = noise2(x_int + 1, y_int, full_seed);
	int32_t u = noise2(x_int, y_int + 1, full_seed);
	int32_t v = noise2(x_int + 1, y_int + 1, full_seed);
	float low = smooth_inter(s, t, x_frac);
	float high = smooth_inter(u, v, x_frac);
	return smooth_inter(low, high, y_frac);
}

float perlin(uint32_t full_seed, float x, float y, float freq, int32_t depth)
{
	float xa = x * freq;
	float ya = y * freq;
	float amp = 1.0;
	float fin = 0;
	float div = 0.0;

	int32_t i;
	for(i = 0; i < depth; i++)
	{
		div += 256 * amp;
		fin += noise2d(xa, ya, full_seed) * amp;
		amp /= 2;
		xa *= 2;
		ya *= 2;
	}

	return fin/div;
}
int32_t perlinint32(int32_t x, int32_t y, uint32_t full_seed) {
	float f_result = perlin(full_seed, x + 0.1, y - 0.1, 0.0069420, 9) * 1000000;
	return (int32_t) f_result;
}
int32_t get_earth_humidity(struct Locator lc, int32_t seed, int32_t x, int32_t y) {
	uint32_t full_seed = seed ^ (lc.surface ^ (lc.orbit ^ (lc.planetary_system  ^ (lc.galaxy))));
	return perlinint32(x, y, full_seed) / 10000;
}
int32_t get_earth_temp(struct Locator lc, int32_t seed, int32_t x, int32_t y) {
	uint32_t full_seed = seed ^ (lc.surface ^ (lc.orbit ^ (lc.planetary_system  ^ (lc.galaxy))));
	return perlinint32(x, y, ~full_seed) / 33333; // one million divided by 33333 is approximately 60, so 60 is the temperature variability of earth. 10 is 0°C. 60 is 50°. 0 is -10° 
}


void gen_chunk_earth_surface(int32_t seed, struct LoadedChunk* chunk) {
	int32_t i, j, curr_tile_temp, curr_tile_humidity, curr_height;
	for (i = 0; i < CHUNK_N_TILES; i++) {
		for (j = 0; j < CHUNK_N_TILES; j++) {
			curr_tile_temp = get_earth_temp(chunk->location, seed, chunk->start_pos_x + j, chunk->start_pos_y + i);
			curr_tile_humidity = get_earth_humidity(chunk->location, seed, chunk->start_pos_y + j + 1, chunk->start_pos_x + i - 1);
			curr_height = perlinint32(chunk->start_pos_x + j, chunk->start_pos_y + i, seed) / 41666; // 24 possible layers, the first 8 are undeground
			if ((curr_tile_humidity > 16 && curr_height < 8) && curr_tile_temp > 5) { // generate water
				chunk->tile[j][i] = 48; // generate water
			}
			else if (curr_height <= 8 && curr_tile_temp <= 9) {
				chunk->tile[j][i] = 73; // generate ice on top of sea / river
			}
			else if (curr_height > 8 && curr_height < 16 && curr_tile_temp <= 9) {
				chunk->tile[j][i] = 81 + curr_height - 8; // generate snow on a dry or cold enough place
			}
			else if (curr_height >= 8 && curr_height < 20 && curr_tile_humidity > 15 && curr_tile_temp > 10) {
				chunk->tile[j][i] = 1 + curr_height - 8; // generate grass
			}
			else if (curr_height >= 8 && curr_height < 20 && curr_tile_humidity < 15 && curr_tile_humidity > 10 && curr_tile_temp > 10) {
				chunk->tile[j][i] = 9 + curr_height - 8; // generate dirt, drier than grass
			}
			else if (curr_height >= 8 && curr_height < 20 && curr_tile_humidity < 10 && curr_tile_temp > 10) {
				chunk->tile[j][i] = 25 + curr_height - 8; // generate sand, drier than dirt
			}
			else if (curr_height >= 16 && curr_tile_temp > 10) {
				chunk->tile[j][i] = 17 + curr_height - 16; // generate stone on mountains (height higher or equal to 20) 
			}
		}
	}
}

	void gen_chunk_earth_orbit(int32_t seed, struct LoadedChunk* chunk) {
		switch (chunk->location.surface) {
			case 0:
				gen_chunk_earth_surface(seed, chunk);
				break;
			case 64:
				// gen_chunk_e_moon_surface(seed, chunk);
				break;
		}
	}
	void gen_chunk_universe(int32_t seed, struct LoadedChunk* chunk){
		if (!chunk->location.galaxy) {
			// gen_chunk_void_universe(seed, chunk);
		}
		else if (chunk->location.galaxy && !chunk->location.planetary_system && !chunk->location.orbit && !chunk->location.surface && !chunk->location.subsurface) {
			// gen_chunk_galaxy(seed, chunk);
		}
		else if(chunk->location.galaxy && chunk->location.planetary_system && !chunk->location.orbit && !chunk->location.surface && !chunk->location.subsurface) {
			// gen_chunk_planetary_system(seed, chunk);
		}
		else if(chunk->location.galaxy && chunk->location.planetary_system && chunk->location.orbit == 4) {
			// the four orbit is earth, upper if statements are pickier because they exclude vehicles for the sake of only giving the corresponding space background
			gen_chunk_earth_orbit(seed, chunk);
		} 
	}

	void gen_chunk(int32_t seed, struct LoadedChunk* chunk) {

		switch(chunk->location.dimension) {
			case 0:
				gen_chunk_universe(seed, chunk);
				break;
			case 1:
				// gen_chunk_heaven(seed, chunk);
				break;
			case -1:
				// gen_chunk_hell(seed, chunk);
				break;
		}
	}

	void gen_spawn_areas(int32_t n_players) {

	}
