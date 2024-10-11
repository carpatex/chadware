#include <stdio.h>
#include <time.h>
#define TPS_OBJECTIVE 20.0
#define NEXT_TICK CLOCKS_PER_SEC / TPS_OBJECTIVE + .01 
void operacion() { //algo random que suma hasta una suma determinada 
	int i, j;
	j = 0;
	while (j < 5000)
		j++;
}

int main() {
	int delta, oldtime;
	double tps, next_tick;
	int ops_per_second = 0;
	while (1) {
		delta = clock() - oldtime;
		tps = (1.0 / delta) * CLOCKS_PER_SEC;
		oldtime = clock();
		while (clock() < oldtime + NEXT_TICK) {
			operacion();
			ops_per_second++;
		}
		printf("delta: %d | op_hechas: %d | tps: %lf\n", delta, ops_per_second, tps);
		ops_per_second = 0;
	}
	return 0;
}
