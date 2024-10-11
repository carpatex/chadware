#include <stdio.h>
#include <time.h>
#define TPS_OBJECTIVE 20
#define NEXT_TICK CLOCKS_PER_SEC / TPS_OBJECTIVE 
void tick() {
	int j;
	j = 0;
	while (j < 777)
		j++;
}

int main() {
	int delta, oldtime;
	double tps;
	while (1) {
		delta = clock() - oldtime;
		tps = (1.0 / delta) * CLOCKS_PER_SEC;
		oldtime = clock();
		tick();
		while(clock() < oldtime + NEXT_TICK);
		printf("delta: %d | tps: %lf\n", delta, tps);
	}
	return 0;
}
