#include "chadware.h"
#include "chadgraphics.h"

char i14toh(int32_t n, int32_t start_point) {
	n -= start_point;
	if (n > 15)
		n %= 16;
	switch(n) {
		case 0:
			return '0';
			break;
		case 1:
			return '1';
			break;
		case 2:
			return '2';
			break;
		case 3:
			return '3';
			break;
		case 4:
			return '4';
			break;
		case 5:
			return '5';
		case 6:
			return '6';
			break;
		case 7:
			return '7';
			break;
		case 8:
			return '8';
			break;
		case 9:
			return '9';
			break;
		case 10:
			return 'a';
			break;
		case 11:
			return 'b';
			break;
		case 12:
			return 'c';
		case 13:
			return 'd';
			break;
		case 14:
			return 'e';
			break;
		case 15:
			return 'f';
			break;
	}
}

void print_grass(int8_t n_anim, int x, int y, int level) {
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	wattr_set(main_win, A_BOLD, 1, NULL);
	if (n_anim > 48) {
		mvwaddch(main_win, y, x, i14toh(1 + level, 1));
	}
	else { 
		mvwaddch(main_win, y, x, '~');
	}
	wattr_set(main_win, A_NORMAL, 0, NULL);
}
