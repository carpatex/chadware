#include "chadware.h"

int main(){
	int i, j, curr_noise;
	srand(time(NULL) ^ ~clock());
	int seed = rand();
	for (i = 0; i < 16; i++){
		for (j = 0; j < 16; j++){
			curr_noise = perlinint32(j, i, seed);
			if(curr_noise / 100)
				printf("%d ", curr_noise);
			else if (curr_noise / 10)
				printf("%d  ", curr_noise);
			else
			 printf("%d   ", curr_noise);
		}
		putchar('\n');
	}
	return 0;
}
