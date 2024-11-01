#include "chadware.h"
static int32_t noise_hash[] = {208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
                     185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
                     9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
                     70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
                     203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
                     164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
                     228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
                     232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
                     193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
                     101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
                     135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
                     114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219};

int32_t noise2(int32_t x, int32_t y, uint32_t full_seed)
{
    int32_t tmp = noise_hash[(y + full_seed) % 256];
    return noise_hash[(tmp + x) % 256];
}

float lin_inter(float x, float y, float s)
{
    return x + s * (y-x);
}

float smooth_inter(float x, float y, float s)
{
    return lin_inter(x, y, s * s * (3-2*s));
}

float noise2d(float x, float y, uint32_t full_seed)
{
    int32_t x_int = x;
    int32_t y_int = y;
    float x_frac = x - x_int;
    float y_frac = y - y_int;
    int32_t s = noise2(x_int, y_int, full_seed);
    int32_t t = noise2(x_int+1, y_int, full_seed);
    int32_t u = noise2(x_int, y_int+1, full_seed);
    int32_t v = noise2(x_int+1, y_int+1, full_seed);
    float low = smooth_inter(s, t, x_frac);
    float high = smooth_inter(u, v, x_frac);
    return smooth_inter(low, high, y_frac);
}

float perlin(uint32_t full_seed, float x, float y, float freq, int32_t depth)
{
    float xa = x*freq;
    float ya = y*freq;
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
	float f_x = x + 0.2;
	float f_y = y + 0.2;
	float f_result = perlin(full_seed, f_x, f_y, 0.1, 2) * 200;
	int32_t result = f_result;
	return result;
}
void gen_spawn_areas(int32_t n_players) {

}